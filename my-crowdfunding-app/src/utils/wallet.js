import wax from './wax';

export const connectWaxWallet = async () => {
  try {
    if (!wax.isAutoLoginAvailable) {
      await wax.login();
    }
    return wax.userAccount;
  } catch (error) {
    console.error('Error connecting to WAX Cloud Wallet:', error);
    throw error;
  }
};