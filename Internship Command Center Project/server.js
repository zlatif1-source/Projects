const http = require("http");
const fs = require("fs");
const path = require("path");
const crypto = require("crypto");

const PORT = process.env.PORT || 3000;
const PUBLIC_DIR = path.join(__dirname, "public");
const DATA_DIR = path.join(__dirname, "data");
const DB_FILE = path.join(DATA_DIR, "applications.json");

const STATUS_ORDER = ["Wishlist", "Applied", "Interview", "Offer", "Rejected"];

function ensureDataFile() {
  if (!fs.existsSync(DATA_DIR)) {
    fs.mkdirSync(DATA_DIR);
  }

  if (!fs.existsSync(DB_FILE)) {
    const seed = [
      {
        id: crypto.randomUUID(),
        company: "Northstar Labs",
        role: "Software Engineering Intern",
        status: "Interview",
        location: "Remote",
        deadline: "2026-06-12",
        appliedDate: "2026-05-20",
        link: "https://example.com/northstar",
        notes: "Prep backend design stories and review API project.",
        priority: "High",
        updatedAt: new Date().toISOString()
      },
      {
        id: crypto.randomUUID(),
        company: "Atlas Health",
        role: "Frontend Intern",
        status: "Applied",
        location: "New York, NY",
        deadline: "2026-06-03",
        appliedDate: "2026-05-22",
        link: "https://example.com/atlas",
        notes: "Follow up after one week if no response.",
        priority: "Medium",
        updatedAt: new Date().toISOString()
      },
      {
        id: crypto.randomUUID(),
        company: "Cedar Cloud",
        role: "Platform Engineering Intern",
        status: "Wishlist",
        location: "Seattle, WA",
        deadline: "2026-06-18",
        appliedDate: "",
        link: "https://example.com/cedar",
        notes: "Tailor resume toward systems and tooling.",
        priority: "High",
        updatedAt: new Date().toISOString()
      }
    ];
    fs.writeFileSync(DB_FILE, JSON.stringify(seed, null, 2));
  }
}

function readApplications() {
  ensureDataFile();
  return JSON.parse(fs.readFileSync(DB_FILE, "utf8"));
}

function writeApplications(applications) {
  fs.writeFileSync(DB_FILE, JSON.stringify(applications, null, 2));
}

function sendJson(res, statusCode, payload) {
  const body = JSON.stringify(payload);
  res.writeHead(statusCode, {
    "Content-Type": "application/json",
    "Content-Length": Buffer.byteLength(body)
  });
  res.end(body);
}

function readBody(req) {
  return new Promise((resolve, reject) => {
    let body = "";
    req.on("data", chunk => {
      body += chunk;
      if (body.length > 1_000_000) {
        req.destroy();
        reject(new Error("Request body is too large."));
      }
    });
    req.on("end", () => {
      try {
        resolve(body ? JSON.parse(body) : {});
      } catch {
        reject(new Error("Invalid JSON."));
      }
    });
    req.on("error", reject);
  });
}

function normalizeApplication(input, existing = {}) {
  const application = {
    id: existing.id || crypto.randomUUID(),
    company: String(input.company || existing.company || "").trim(),
    role: String(input.role || existing.role || "").trim(),
    status: STATUS_ORDER.includes(input.status) ? input.status : existing.status || "Wishlist",
    location: String(input.location || existing.location || "").trim(),
    deadline: String(input.deadline || existing.deadline || "").trim(),
    appliedDate: String(input.appliedDate || existing.appliedDate || "").trim(),
    link: String(input.link || existing.link || "").trim(),
    notes: String(input.notes || existing.notes || "").trim(),
    priority: ["Low", "Medium", "High"].includes(input.priority) ? input.priority : existing.priority || "Medium",
    updatedAt: new Date().toISOString()
  };

  if (!application.company || !application.role) {
    throw new Error("Company and role are required.");
  }

  return application;
}

function getAnalytics(applications) {
  const today = new Date();
  const nextWeek = new Date();
  nextWeek.setDate(today.getDate() + 7);

  const statusCounts = STATUS_ORDER.reduce((counts, status) => {
    counts[status] = applications.filter(app => app.status === status).length;
    return counts;
  }, {});

  const urgentDeadlines = applications.filter(app => {
    if (!app.deadline || ["Offer", "Rejected"].includes(app.status)) {
      return false;
    }
    const deadline = new Date(`${app.deadline}T23:59:59`);
    return deadline >= today && deadline <= nextWeek;
  }).length;

  const active = applications.filter(app => !["Offer", "Rejected"].includes(app.status)).length;
  const interviews = applications.filter(app => app.status === "Interview").length;
  const offers = applications.filter(app => app.status === "Offer").length;

  return {
    total: applications.length,
    active,
    interviews,
    offers,
    urgentDeadlines,
    statusCounts
  };
}

function serveStatic(req, res) {
  const requestedPath = req.url === "/" ? "/index.html" : req.url;
  const filePath = path.normalize(path.join(PUBLIC_DIR, requestedPath));

  if (!filePath.startsWith(PUBLIC_DIR)) {
    res.writeHead(403);
    res.end("Forbidden");
    return;
  }

  fs.readFile(filePath, (err, content) => {
    if (err) {
      res.writeHead(404);
      res.end("Not found");
      return;
    }

    const ext = path.extname(filePath);
    const contentType = {
      ".html": "text/html",
      ".css": "text/css",
      ".js": "text/javascript",
      ".json": "application/json",
      ".svg": "image/svg+xml"
    }[ext] || "text/plain";

    res.writeHead(200, { "Content-Type": contentType });
    res.end(content);
  });
}

async function handleApi(req, res) {
  const url = new URL(req.url, `http://${req.headers.host}`);
  const applications = readApplications();

  if (url.pathname === "/api/applications" && req.method === "GET") {
    const query = url.searchParams.get("q")?.toLowerCase() || "";
    const status = url.searchParams.get("status") || "All";
    const priority = url.searchParams.get("priority") || "All";

    const filtered = applications
      .filter(app => status === "All" || app.status === status)
      .filter(app => priority === "All" || app.priority === priority)
      .filter(app => {
        const haystack = `${app.company} ${app.role} ${app.location} ${app.notes}`.toLowerCase();
        return haystack.includes(query);
      })
      .sort((a, b) => STATUS_ORDER.indexOf(a.status) - STATUS_ORDER.indexOf(b.status) || a.company.localeCompare(b.company));

    sendJson(res, 200, { applications: filtered, analytics: getAnalytics(applications) });
    return;
  }

  if (url.pathname === "/api/applications" && req.method === "POST") {
    try {
      const payload = await readBody(req);
      const application = normalizeApplication(payload);
      applications.push(application);
      writeApplications(applications);
      sendJson(res, 201, { application, analytics: getAnalytics(applications) });
    } catch (error) {
      sendJson(res, 400, { error: error.message });
    }
    return;
  }

  const match = url.pathname.match(/^\/api\/applications\/([^/]+)$/);
  if (match && req.method === "PUT") {
    const id = match[1];
    const index = applications.findIndex(app => app.id === id);

    if (index === -1) {
      sendJson(res, 404, { error: "Application not found." });
      return;
    }

    try {
      const payload = await readBody(req);
      applications[index] = normalizeApplication(payload, applications[index]);
      writeApplications(applications);
      sendJson(res, 200, { application: applications[index], analytics: getAnalytics(applications) });
    } catch (error) {
      sendJson(res, 400, { error: error.message });
    }
    return;
  }

  if (match && req.method === "DELETE") {
    const id = match[1];
    const nextApplications = applications.filter(app => app.id !== id);

    if (nextApplications.length === applications.length) {
      sendJson(res, 404, { error: "Application not found." });
      return;
    }

    writeApplications(nextApplications);
    sendJson(res, 200, { analytics: getAnalytics(nextApplications) });
    return;
  }

  sendJson(res, 404, { error: "API route not found." });
}

const server = http.createServer((req, res) => {
  if (req.url.startsWith("/api/")) {
    handleApi(req, res).catch(error => sendJson(res, 500, { error: error.message }));
    return;
  }

  serveStatic(req, res);
});

if (require.main === module) {
  server.listen(PORT, () => {
    ensureDataFile();
    console.log(`Internship Command Center running at http://localhost:${PORT}`);
  });
}

module.exports = { server, ensureDataFile };
