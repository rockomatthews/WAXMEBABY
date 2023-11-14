import logo from '../assets/logo.png';
import { connectWaxWallet } from '../utils/wallet';

function Navbar() {
  const handleConnectWallet = async () => {
    try {
      const userAccount = await connectWaxWallet();
      console.log('Connected with:', userAccount);
      // Store user account details in state or context for further use
    } catch (error) {
      console.error('Could not connect to WAX Cloud Wallet:', error);
    }
  };

  return (
    <nav className="navbar">
      <img src={logo} alt="Site Logo" className="navbar-logo" />
      <button onClick={handleConnectWallet} className="connect-wallet-button">
        Connect Wallet
      </button>
    </nav>
  );
}

export default Navbar;
