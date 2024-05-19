const ctx = document.getElementById("weather-chart").getContext("2d");
const chart = new Chart(ctx, {
        type: "line",
        data: {
            labels: [],
            datasets: [
                {
                    label: "Temperature (°C)",
                    data: [],
                    backgroundColor: "rgba(255, 99, 132, 0.2)",
                    borderColor: "rgba(255, 99, 132, 1)",
                    borderWidth: 1,
                    yAxisID: "temperature",
                },
                {
                    label: "Humidity (%)",
                    data: [],
                    backgroundColor: "rgba(0, 0, 255, 0.2)",
                    borderColor: "rgba(0, 0, 255, 1)",
                    borderWidth: 1,
                    yAxisID: "humidity",
                },
            ]
        },
        options: {
            scales: {
                temperature: {
                    type: "linear",
                    position: "left",
                    display: true,
                },
                humidity: {
                    type: "linear",
                    position: "right",
                    display: true,
                    grid: {
                        display: false,
                    }
                },
            },
        },
    },
);

async function clearHistory() {
    if (confirm("Are you sure you want to clear the history?")) {
        await fetch("/clear")
        await updateChart()
    }
}

async function updateChart() {
    let response = await fetch("/retrieve")
    let data = await response.json()
    let timestamps = data["timestamps"];
    let temperatureValues = data["temperature_values"]
    let humidityValues = data["humidity_values"];
    let numToShow = Math.max(0, document.getElementById("datapoint-count").value);
    if (numToShow > 0) {
        chart.data.labels = timestamps.slice(Math.max(0, timestamps.length - numToShow))
        chart.data.datasets[0].data = temperatureValues.slice(Math.max(0, temperatureValues.length - numToShow));
        chart.data.datasets[1].data = humidityValues.slice(Math.max(0, humidityValues.length - numToShow));
    } else {
        chart.data.labels = timestamps
        chart.data.datasets[0].data = temperatureValues
        chart.data.datasets[1].data = humidityValues
    }
    if (data["timestamps"].length === 0) {
        document.getElementById("avg_temp").innerText = "__";
        document.getElementById("avg_hum").innerText = "__";
        document.getElementById("max_temp").innerText = "__";
        document.getElementById("min_temp").innerText = "__";
        document.getElementById("max_hum").innerText = "__";
        document.getElementById("min_hum").innerText = "__";
    } else {
        document.getElementById("avg_temp").innerText = (data["temperature_values"].reduce((a, b) => a + b, 0) / data["temperature_values"].length).toFixed(2);
        document.getElementById("avg_hum").innerText = (data["humidity_values"].reduce((a, b) => a + b, 0) / data["humidity_values"].length).toFixed(2);
        document.getElementById("max_temp").innerText = (Math.max(...data["temperature_values"])).toFixed(2);
        document.getElementById("min_temp").innerText = (Math.min(...data["temperature_values"])).toFixed(2);
        document.getElementById("max_hum").innerText = (Math.max(...data["humidity_values"])).toFixed(2);
        document.getElementById("min_hum").innerText = (Math.min(...data["humidity_values"])).toFixed(2);
    }
    chart.update();

    let now = new Date()
    document.getElementById("last_refresh_time").innerText = now.toLocaleDateString() + ", " + now.toLocaleTimeString()
}

updateChart()

// Poll for information to update
setInterval(updateChart, 200)
