import React, { useEffect, useState } from "react";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import {
  faThermometerHalf,
  faGlassWhiskey,
  faTint,
  faWater,
  faWind,
} from "@fortawesome/free-solid-svg-icons";
import "./dashboard.css";
import { Link, useNavigate } from "react-router-dom";
import Aquarium from "../../images/aquarium-tank-with-gold-fishes-white-background_1308-130424.avif";

var channelId = "2538104";
var apiKey = "XXXXXXXXXXXXXXXX";

var url =
  "https://api.thingspeak.com/channels/" +
  channelId +
  "/feeds.json?api_key=" +
  apiKey;

/*

field1 - pH
field2 - tds
field3 - turbidity
field4 - temp

*/
const Dashboard = ({ isAuthenticated }) => {
  // tinkercadData();
  const navigate = useNavigate();
  const [temp, setTemp] = useState(null);
  const [TDS, setTDS] = useState(null);
  const [turbidity, setTurbidity] = useState(null);
  const [pH, setPH] = useState(null);
  const [oxygen, setOxygen] = useState(null);

  if (!isAuthenticated) navigate("/");
  useEffect(() => {
    const fetchData = async () => {
      try {
        const response = await fetch(url);
        const data = await response.json();
        const feeds = data.feeds[data.feeds.length - 1];
        console.log(data);
        setPH(feeds.field1);
        setTDS(feeds.field2);
        setTurbidity(feeds.field3);
        setTemp(feeds.field4);
        setOxygen(feeds.field5);

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
    <>
      <div className="dashboard">
        <div className="header">Home / Dashboard</div>
        <div className="card-container">
          <div className="card">
            <Link to="/dashboard/tempChart">
              <div className="card-title">TEMPERATURE</div>
              <div className="card-value">
                {temp} <span className="celsius">°c</span>
              </div>
              <div className="card-icon">
                <FontAwesomeIcon icon={faThermometerHalf} />
              </div>
              <div className="card-footer">0.2% (6.36) past hour</div>
            </Link>
          </div>
          <div className="card">
            <Link to="/dashboard/tdsChart">
              <div className="card-title">TDS</div>
              <div className="card-value">
                {TDS} <span className="unit">NTU</span>
              </div>
              <div className="card-icon">
                <FontAwesomeIcon icon={faGlassWhiskey} />
              </div>
              <div className="card-footer">0.7% (3.28) past hour</div>
            </Link>
          </div>
          <div className="card">
            <Link to="/dashboard/pHChart">
              <div className="card-title">pH</div>
              <div className="card-value">{pH}</div>
              <div className="card-icon">
                <FontAwesomeIcon icon={faTint} />
              </div>
              <div className="card-footer">0.8% (5.25) past hour</div>
            </Link>
          </div>
          <div className="card">
            <Link to="/dashboard/turbidityChart">
              <div className="card-title">Turbidity</div>
              <div className="card-value">
                {turbidity} <span className="unit">cm</span>
              </div>
              <div className="card-icon">
                <FontAwesomeIcon icon={faWater} />
              </div>
              <div className="card-footer">4.1% (76.0) past hour</div>
            </Link>
          </div>
          <div className="card">
            <Link to="/dashboard/oxygenChart">
              <div className="card-title">Oxygen</div>
              <div className="card-value">
                {oxygen} <span className="unit">ppm</span>
              </div>
              <div className="card-icon">
                <FontAwesomeIcon icon={faWind} />
              </div>
              <div className="card-footer">4.1% (76.0) past hour</div>
            </Link>
          </div>
        </div>
      </div>
      <div class="ocean">
        <div class="bubble bubble--1"></div>
        <div class="bubble bubble--2"></div>
        <div class="bubble bubble--3"></div>
        <div class="bubble bubble--4"></div>
        <div class="bubble bubble--5"></div>
        <div class="bubble bubble--6"></div>
        <div class="bubble bubble--7"></div>
        <div class="bubble bubble--8"></div>
        <div class="bubble bubble--9"></div>
        <div class="bubble bubble--10"></div>
        <div class="bubble bubble--11"></div>
        <div class="bubble bubble--12"></div>
        <div id="fish1"></div>
        <div id="fish2"></div>
        <div id="fish3"></div>
      </div>
      <img src={Aquarium} alt="" />
    </>
  );
};

export default Dashboard;
