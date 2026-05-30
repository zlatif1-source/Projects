const state = {
  applications: [],
  analytics: null,
  filters: {
    q: "",
    status: "All",
    priority: "All"
  }
};

const elements = {
  form: document.querySelector("#applicationForm"),
  applicationId: document.querySelector("#applicationId"),
  company: document.querySelector("#company"),
  role: document.querySelector("#role"),
  status: document.querySelector("#status"),
  priority: document.querySelector("#priority"),
  location: document.querySelector("#location"),
  deadline: document.querySelector("#deadline"),
  appliedDate: document.querySelector("#appliedDate"),
  link: document.querySelector("#link"),
  notes: document.querySelector("#notes"),
  formTitle: document.querySelector("#formTitle"),
  resetButton: document.querySelector("#resetButton"),
  newApplicationButton: document.querySelector("#newApplicationButton"),
  searchInput: document.querySelector("#searchInput"),
  statusFilter: document.querySelector("#statusFilter"),
  priorityFilter: document.querySelector("#priorityFilter"),
  applicationList: document.querySelector("#applicationList"),
  statusStrip: document.querySelector("#statusStrip"),
  template: document.querySelector("#applicationCardTemplate"),
  totalApplications: document.querySelector("#totalApplications"),
  activeApplications: document.querySelector("#activeApplications"),
  interviews: document.querySelector("#interviews"),
  urgentDeadlines: document.querySelector("#urgentDeadlines")
};

function buildQuery() {
  const params = new URLSearchParams();
  Object.entries(state.filters).forEach(([key, value]) => {
    if (value) {
      params.set(key, value);
    }
  });
  return params.toString();
}

async function loadApplications() {
  const response = await fetch(`/api/applications?${buildQuery()}`);
  const data = await response.json();
  state.applications = data.applications;
  state.analytics = data.analytics;
  render();
}

async function saveApplication(application) {
  const isEditing = Boolean(application.id);
  const response = await fetch(isEditing ? `/api/applications/${application.id}` : "/api/applications", {
    method: isEditing ? "PUT" : "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(application)
  });

  const data = await response.json();
  if (!response.ok) {
    alert(data.error || "Something went wrong.");
    return;
  }

  resetForm();
  await loadApplications();
}

async function deleteApplication(id) {
  const confirmed = window.confirm("Delete this application?");
  if (!confirmed) {
    return;
  }

  await fetch(`/api/applications/${id}`, { method: "DELETE" });
  await loadApplications();
}

function getFormData() {
  return {
    id: elements.applicationId.value,
    company: elements.company.value,
    role: elements.role.value,
    status: elements.status.value,
    priority: elements.priority.value,
    location: elements.location.value,
    deadline: elements.deadline.value,
    appliedDate: elements.appliedDate.value,
    link: elements.link.value,
    notes: elements.notes.value
  };
}

function editApplication(application) {
  elements.applicationId.value = application.id;
  elements.company.value = application.company;
  elements.role.value = application.role;
  elements.status.value = application.status;
  elements.priority.value = application.priority;
  elements.location.value = application.location;
  elements.deadline.value = application.deadline;
  elements.appliedDate.value = application.appliedDate;
  elements.link.value = application.link;
  elements.notes.value = application.notes;
  elements.formTitle.textContent = "Edit role";
  window.scrollTo({ top: 0, behavior: "smooth" });
}

function resetForm() {
  elements.form.reset();
  elements.applicationId.value = "";
  elements.priority.value = "Medium";
  elements.status.value = "Wishlist";
  elements.formTitle.textContent = "Add role";
}

function formatDate(value) {
  if (!value) {
    return "Not set";
  }

  return new Intl.DateTimeFormat("en", {
    month: "short",
    day: "numeric",
    year: "numeric"
  }).format(new Date(`${value}T00:00:00`));
}

function renderMetrics() {
  const analytics = state.analytics || {};
  elements.totalApplications.textContent = analytics.total || 0;
  elements.activeApplications.textContent = analytics.active || 0;
  elements.interviews.textContent = analytics.interviews || 0;
  elements.urgentDeadlines.textContent = analytics.urgentDeadlines || 0;
}

function renderStatusStrip() {
  const counts = state.analytics?.statusCounts || {};
  const statuses = ["All", "Wishlist", "Applied", "Interview", "Offer", "Rejected"];
  elements.statusStrip.innerHTML = "";

  statuses.forEach(status => {
    const button = document.createElement("button");
    const count = status === "All" ? state.analytics?.total || 0 : counts[status] || 0;
    button.textContent = `${status} (${count})`;
    button.classList.toggle("active", state.filters.status === status);
    button.addEventListener("click", () => {
      state.filters.status = status;
      elements.statusFilter.value = status;
      loadApplications();
    });
    elements.statusStrip.append(button);
  });
}

function renderApplications() {
  elements.applicationList.innerHTML = "";

  if (!state.applications.length) {
    const empty = document.createElement("div");
    empty.className = "empty-state";
    empty.textContent = "No applications match your filters.";
    elements.applicationList.append(empty);
    return;
  }

  state.applications.forEach(application => {
    const card = elements.template.content.firstElementChild.cloneNode(true);
    card.querySelector("h3").textContent = application.company;
    card.querySelector(".role-line").textContent = application.role;
    card.querySelector(".status-pill").textContent = application.status;
    const priority = card.querySelector(".priority-pill");
    priority.textContent = `${application.priority} priority`;
    priority.classList.add(application.priority.toLowerCase());
    card.querySelector(".location").textContent = application.location || "Not set";
    card.querySelector(".deadline").textContent = formatDate(application.deadline);
    card.querySelector(".applied").textContent = formatDate(application.appliedDate);
    card.querySelector(".notes").textContent = application.notes || "No notes yet.";

    const link = card.querySelector(".role-link");
    if (application.link) {
      link.href = application.link;
    } else {
      link.remove();
    }

    card.querySelector(".edit-button").addEventListener("click", () => editApplication(application));
    card.querySelector(".delete-button").addEventListener("click", () => deleteApplication(application.id));
    elements.applicationList.append(card);
  });
}

function render() {
  renderMetrics();
  renderStatusStrip();
  renderApplications();
}

function wireEvents() {
  elements.form.addEventListener("submit", event => {
    event.preventDefault();
    saveApplication(getFormData());
  });

  elements.resetButton.addEventListener("click", resetForm);
  elements.newApplicationButton.addEventListener("click", () => {
    resetForm();
    elements.company.focus();
  });

  elements.searchInput.addEventListener("input", event => {
    state.filters.q = event.target.value;
    loadApplications();
  });

  elements.statusFilter.addEventListener("change", event => {
    state.filters.status = event.target.value;
    loadApplications();
  });

  elements.priorityFilter.addEventListener("change", event => {
    state.filters.priority = event.target.value;
    loadApplications();
  });
}

wireEvents();
loadApplications();
