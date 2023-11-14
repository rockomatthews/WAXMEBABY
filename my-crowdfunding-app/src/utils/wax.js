import { WaxJS } from '@waxio/waxjs/dist';

const wax = new WaxJS({
  rpcEndpoint: 'https://wax.greymass.com', // WAX blockchain RPC endpoint
  tryAutoLogin: false, // set to true if you want to try auto-login
});

export default wax;