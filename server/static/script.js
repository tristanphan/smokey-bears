const ctx = document.getElementById("weather-chart").getContext("2d");
const chart = new Chart(ctx, {
    type: "line", data: {
        labels: [], datasets: [{
            label: "Temperature (°C)",
            data: [],
            backgroundColor: "rgba(255, 99, 132, 0.2)",
            borderColor: "rgba(255, 99, 132, 1)",
            borderWidth: 1,
            yAxisID: "temperature",
        }, {
            label: "Gas Level (%)",
            data: [],
            backgroundColor: "rgba(0, 0, 255, 0.2)",
            borderColor: "rgba(0, 0, 255, 1)",
            borderWidth: 1,
            yAxisID: "gas_level",
        },]
    }, options: {
        scales: {
            temperature: {
                type: "linear", position: "left", display: true,
            }, gas_level: {
                type: "linear", position: "right", display: true, grid: {
                    display: false,
                }
            },
        },
    },
},);

async function clearHistory() {
    if (confirm("Are you sure you want to clear the history?")) {
        await fetch("/clear")
        await updateChart()
    }
}

async function turnOffAlarm() {
    if (confirm("Are you sure you want to deactivate the alarm?")) {
        await fetch("/set_alarm?status=off")
        await updateChart()
    }
}

async function turnOnAlarm() {
    if (confirm("Are you sure you want to activate the alarm?")) {
        await fetch("/set_alarm?status=on")
        await updateChart()
    }
}

async function setTemperatureThreshold(newTemperatureThreshold) {
    await fetch(`/set_thresholds?temperature=${newTemperatureThreshold}`)
}

async function setGasLevelThreshold(newGasLevelThreshold) {
    await fetch(`/set_thresholds?gas_level=${newGasLevelThreshold}`)
}

async function updateChart() {
    let response = await fetch("/retrieve")
    let data = await response.json()

    // Extract request
    let timestamps = data["timestamps"];
    let temperatureValues = data["temperature_values"]
    let gasLevelValues = data["gas_level_values"];
    let alarmStatus = data["alarm_status"];

    // Set chart datapoints
    let numToShow = Math.max(0, document.getElementById("datapoint-count").value);
    if (numToShow > 0) {
        chart.data.labels = timestamps.slice(Math.max(0, timestamps.length - numToShow))
        chart.data.datasets[0].data = temperatureValues.slice(Math.max(0, temperatureValues.length - numToShow));
        chart.data.datasets[1].data = gasLevelValues.slice(Math.max(0, gasLevelValues.length - numToShow));
    } else {
        chart.data.labels = timestamps
        chart.data.datasets[0].data = temperatureValues
        chart.data.datasets[1].data = gasLevelValues
    }

    // Set statistics
    if (data["timestamps"].length === 0) {
        document.getElementById("avg_temp").innerText = "__";
        document.getElementById("avg_hum").innerText = "__";
        document.getElementById("max_temp").innerText = "__";
        document.getElementById("min_temp").innerText = "__";
        document.getElementById("max_hum").innerText = "__";
        document.getElementById("min_hum").innerText = "__";
    } else {
        document.getElementById("avg_temp").innerText = (data["temperature_values"].reduce((a, b) => a + b, 0) / data["temperature_values"].length).toFixed(2);
        document.getElementById("avg_hum").innerText = (data["gas_level_values"].reduce((a, b) => a + b, 0) / data["gas_level_values"].length).toFixed(2);
        document.getElementById("max_temp").innerText = (Math.max(...data["temperature_values"])).toFixed(2);
        document.getElementById("min_temp").innerText = (Math.min(...data["temperature_values"])).toFixed(2);
        document.getElementById("max_hum").innerText = (Math.max(...data["gas_level_values"])).toFixed(2);
        document.getElementById("min_hum").innerText = (Math.min(...data["gas_level_values"])).toFixed(2);
    }
    chart.update();

    // Set refresh date
    let now = new Date()
    document.getElementById("last_refresh_time").innerText = now.toLocaleDateString() + ", " + now.toLocaleTimeString()

    // Set alarm buttons
    document.getElementById("alarm-on-button").hidden = alarmStatus
    document.getElementById("alarm-off-button").hidden = !alarmStatus
    document.getElementById("alarm-text").hidden = !alarmStatus
}

void updateChart()

// Poll for information to update
setInterval(updateChart, 200)

window.addEventListener("resize", () => {
    chart.resize();
});

document.getElementById("temperature-threshold").addEventListener("keydown", async (event) => {
    let value = document.getElementById("temperature-threshold").value
    if (event.key === "Enter" && value.length > 0) {
        await setTemperatureThreshold(value)
    }
})

document.getElementById("gas-level-threshold").addEventListener("keydown", async (event) => {
    let value = document.getElementById("gas-level-threshold").value
    if (event.key === "Enter" && value.length > 0) {
        await setGasLevelThreshold(value)
    }
})