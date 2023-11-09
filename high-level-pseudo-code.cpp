class CrowdfundingContract : public eosio::contract
{
public:
    using contract::contract;

    // Action to create a new campaign
    [[eosio::action]] void createcamp(const eosio::name &creator, const std::string &title, const std::string &description, const eosio::asset &goal_amount, const uint64_t &deadline);

    // Action for a user to contribute to a campaign
    [[eosio::action]] void contribute(const eosio::name &contributor, const uint64_t &campaign_id, const eosio::asset &amount);

    // Action to withdraw funds by the creator if the campaign is successful
    [[eosio::action]] void withdraw(const eosio::name &creator, const uint64_t &campaign_id);

    // Action to refund contributions if the campaign is not successful
    [[eosio::action]] void refund(const eosio::name &contributor, const uint64_t &campaign_id);

    // Action to get the list of campaigns
    [[eosio::action]] void getcampaigns();

    // Action to get details of a specific campaign
    [[eosio::action]] void getcampdet(const uint64_t &campaign_id);

    // Action to report a campaign
    [[eosio::action]] void reportcamp(const eosio::name &reporter, const uint64_t &campaign_id);

    // Admin actions...
};
