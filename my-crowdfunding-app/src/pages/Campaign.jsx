import React from 'react';
import { useParams } from 'react-router-dom';

function Campaign() {
  let { id } = useParams();
  // Fetch campaign details using the id and display them

  return (
    <div>
      <h1>Campaign Details</h1>
      {/* Render campaign details and contribution form here */}
    </div>
  );
}

export default Campaign;
