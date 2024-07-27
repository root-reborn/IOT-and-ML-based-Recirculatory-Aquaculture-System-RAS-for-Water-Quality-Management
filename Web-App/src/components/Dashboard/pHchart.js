import React, { useEffect, useState } from "react";
import { Line } from "react-chartjs-2";
import "chart.js/auto"; // Import the bundled version of Chart.js
import { Link } from "react-router-dom";
import "./dashboard.css";

var channelId = "2538104";
var apiKey = "XXXXXXXXXXXXXXXX";

var url =
  "https://api.thingspeak.com/channels/" +
  channelId +
  "/feeds.json?api_key=" +
  apiKey;

const Ph = () => {
  const [chartData, setChartData] = useState({ labels: [], datasets: [] });

  useEffect(() => {
    const fetchData = async () => {
      try {
        const response = await fetch(url);
        const data = await response.json();
        const feeds = data.feeds.slice(-30);

        const pH = feeds.map((feed) => ({
          x: new Date(feed.created_at).toLocaleTimeString(),
          y: feed.field1, // Assuming field1 contains temperature data
        }));

        setChartData({
          labels: pH.map((temp) => temp.x),
          datasets: [
            {
              label: "pH",
              data: pH.map((temp) => temp.y),
              fill: false,
              borderColor: "rgb(75, 192, 192)",
              tension: 0.1,
            },
          ],
        });

        console.log(data.feeds);
      } catch (e) {
        console.error("Error fetching data:", e);
      }
    };

    fetchData();
    const intervalId = setInterval(fetchData, 15000);

    return () => clearInterval(intervalId);
  }, []);

  return (
    <div className="dashboard">
      <div className="header">
        Home / <Link to="/dashboard">Dashboard</Link> / pH{" "}
      </div>
      <div style={{ margin: "50px auto", width: "1000px", height: "800px" }}>
        <Line data={chartData} />
      </div>
    </div>
  );
};

export default Ph;
