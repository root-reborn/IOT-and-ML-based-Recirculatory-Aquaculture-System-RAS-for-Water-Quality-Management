import React, { useState } from "react";
import LoginComponent from "./components/Login/login";
import Header from "./components/header";
import Dashboard from "./components/Dashboard/dashboard";
import Temperature from "./components/Dashboard/tempChart";
import "bootstrap/dist/css/bootstrap.min.css";
import "@fortawesome/fontawesome-svg-core/styles.css";
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import Ph from "./components/Dashboard/pHchart";
import TDS from "./components/Dashboard/tdsChart";
import Turbidity from "./components/Dashboard/turbidity";
import Oxygen from "./components/Dashboard/oxygenChart";

const App = () => {
  const [isAuthenticated, setAuthenticated] = useState(
    localStorage.getItem("isAuthenticated") === "true"
  );

  return (
    <Router>
      <Header />
      <Routes>
        <Route
          path="/"
          element={<LoginComponent setAuthenticated={setAuthenticated} />}
        />
        <Route
          path="/dashboard"
          element={<Dashboard isAuthenticated={isAuthenticated} />}
        />
        <Route path="/dashboard/tempChart" element={<Temperature />} />
        <Route path="/dashboard/pHChart" element={<Ph />} />
        <Route path="/dashboard/tdsChart" element={<TDS />} />
        <Route path="/dashboard/turbidityChart" element={<Turbidity />} />
        <Route path="/dashboard/oxygenChart" element={<Oxygen />} />
      </Routes>
    </Router>
  );
};

export default App;
