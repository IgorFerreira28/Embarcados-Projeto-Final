const BASE_URL = "http://127.0.0.1:5000";

// Obter o valor atual do threshold
async function getThreshold() {
  try {
    const response = await fetch(`${BASE_URL}/controle/get_config`);
    if (!response.ok) throw new Error("Erro ao obter o threshold");

    const threshold = await response.json();
    document.getElementById("current-threshold").value = threshold;
  } catch (error) {
    alert(error.message);
  }
}

// Atualizar o valor do threshold
async function setThreshold() {
  const newThreshold = document.getElementById("new-threshold").value;

  if (!newThreshold) {
    alert("Por favor, insira um valor válido.");
    return;
  }

  try {
    const response = await fetch(`${BASE_URL}/controle/set_config`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ threshold: parseInt(newThreshold, 10) }),
    });

    if (!response.ok) throw new Error("Erro ao atualizar o threshold");

    const result = await response.json();
    alert(result.mensagem);
    getThreshold();
  } catch (error) {
    alert(error.message);
  }
}

// Obter logs registrados
async function getLogs() {
  try {
    const response = await fetch(`${BASE_URL}/logging/get_logs`);
    if (!response.ok) throw new Error("Erro ao obter os logs");

    const logs = await response.json();
    const logsContainer = document.getElementById("logs");
    logsContainer.innerHTML = "";

    logs.forEach((log) => {
      const logElement = document.createElement("div");
      logElement.className = "log-item";
      logElement.innerText = `ID: ${log[0]} | Valor: ${log[1]} | Timestamp: ${log[2]}`;
      logsContainer.appendChild(logElement);
    });
  } catch (error) {
    alert(error.message);
  }
}
