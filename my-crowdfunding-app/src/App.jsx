import React from 'react';
import Navbar from './components/Navbar';
import { Routes, Route } from 'react-router-dom';
import Home from './pages/Home';
import Campaign from './pages/Campaign';
import CreateCampaign from './pages/CreateCampaign';
import Dashboard from './pages/Dashboard';

function App() {
  const handleConnectWallet = () => {
    // Logic to connect to the WAX wallet
    console.log('Connect to WAX wallet');
  };

  return (
    <div>
      <Navbar onConnectWallet={handleConnectWallet} />
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/campaign/:id" element={<Campaign />} />
        <Route path="/create" element={<CreateCampaign />} />
        <Route path="/dashboard" element={<Dashboard />} />
        {/* Add other routes here */}
      </Routes>
    </div>
  );
}

export default App;
