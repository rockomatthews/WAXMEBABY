import React, { useEffect, useState } from 'react';
import { getCampaigns } from '../services/campaignService';

function Home() {
  const [campaigns, setCampaigns] = useState([]);

  useEffect(() => {
    async function fetchCampaigns() {
      const campaignsData = await getCampaigns();
      setCampaigns(campaignsData);
    }

    fetchCampaigns();
  }, []);

  return (
    <div>
      <h1>Active Campaigns</h1>
      {/* Render campaigns here */}
    </div>
  );
}

export default Home;
