$node = "$env:USERPROFILE\.cache\codex-runtimes\codex-primary-runtime\dependencies\node\bin\node.exe"

if (-not (Test-Path $node)) {
  Write-Error "Could not find the bundled Node runtime at $node"
  exit 1
}

& $node server.js
