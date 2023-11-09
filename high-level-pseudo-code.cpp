#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

class [[eosio::contract("crowdfunding")]] crowdfunding : public contract
{
public:
    using contract::contract;

    // Constructor
    crowdfunding(name receiver, name code, datastream<const char *> ds)
        : contract(receiver, code, ds), _campaigns(receiver, receiver.value) {}

    // Table to store campaigns
    struct [[eosio::table]] campaign
    {
        uint64_t id;
        name creator;
        std::string title;
        std::string description;
        std::string video_url; // Field to store the video URL
        asset goal_amount;
        time_point_sec deadline;
        asset current_amount;
        bool is_active;

        uint64_t primary_key() const { return id; }
        uint64_t by_creator() const { return creator.value; }
    };

    typedef multi_index<"campaigns"_n, campaign,
                        indexed_by<"bycreator"_n, const_mem_fun<campaign, uint64_t, &campaign::by_creator>>>
        campaign_index;

    campaign_index _campaigns;

    // Action to create a new campaign
    [[eosio::action]] void createcamp(const name &creator, const std::string &title, const std::string &description, const std::string &video_url, const asset &goal_amount, const time_point_sec &deadline)
    {
        require_auth(creator);

        // Validate input
        check(goal_amount.symbol.is_valid(), "Invalid symbol name");
        check(goal_amount.amount > 0, "Goal amount must be positive");
        check(deadline.utc_seconds > current_time_point().sec_since_epoch(), "Deadline must be in the future");
        check(description.size() <= 1024, "Description is too long"); // Check the length of the description
        check(video_url.size() <= 256, "Video URL is too long");      // Check the length of the video URL
        // Additional validation for the video URL format can be added here if needed

        // Create new campaign
        _campaigns.emplace(creator, [&](auto &new_campaign)
                           {
            new_campaign.id = _campaigns.available_primary_key();
            new_campaign.creator = creator;
            new_campaign.title = title;
            new_campaign.description = description;
            new_campaign.video_url = video_url; // Store the video URL
            new_campaign.goal_amount = goal_amount;
            new_campaign.deadline = deadline;
            new_campaign.current_amount = asset(0, goal_amount.symbol);
            new_campaign.is_active = true; });
    }

    // Listen for transfer notifications from the eosio.token contract
    [[eosio::on_notify("eosio.token::transfer")]] void on_transfer(const name &from, const name &to, const asset &amount, const std::string &memo)
    {
        // Ensure this is an incoming transfer to the crowdfunding contract
        if (to != get_self())
            return;

        // Parse the memo to extract the campaign_id
        // In a real-world scenario, you'd want to add error checking here
        uint64_t campaign_id = std::stoull(memo);

        // Find the campaign
        auto campaign_itr = _campaigns.find(campaign_id);
        check(campaign_itr != _campaigns.end(), "Campaign does not exist");
        check(campaign_itr->is_active, "Campaign is not active");
        check(campaign_itr->deadline.utc_seconds > current_time_point().sec_since_epoch(), "Campaign deadline has passed");
        check(campaign_itr->goal_amount.symbol == amount.symbol, "Contribution symbol does not match campaign goal symbol");

        // Update the campaign's current amount
        _campaigns.modify(campaign_itr, get_self(), [&](auto &mod_campaign)
                          { mod_campaign.current_amount += amount; });
    }

    // Action to withdraw funds by the creator if the campaign is successful
    [[eosio::action]] void withdraw(const name &creator, const uint64_t &campaign_id)
    {
        require_auth(creator);

        // Find the campaign
        auto campaign_itr = _campaigns.find(campaign_id);
        check(campaign_itr != _campaigns.end(), "Campaign does not exist");
        check(campaign_itr->creator == creator, "Only the campaign creator can withdraw funds");
        check(campaign_itr->is_active, "Campaign is not active");
        check(campaign_itr->deadline.utc_seconds <= current_time_point().sec_since_epoch(), "Campaign deadline has not passed yet");
        check(campaign_itr->current_amount >= campaign_itr->goal_amount, "Campaign has not reached its funding goal");

        // Transfer funds from the contract to the creator
        action(
            permission_level{get_self(), "active"_n},
            "eosio.token"_n, "transfer"_n,
            std::make_tuple(get_self(), creator, campaign_itr->current_amount, std::string("Campaign funding withdrawal")))
            .send();

        // Deactivate the campaign
        _campaigns.modify(campaign_itr, same_payer, [&](auto &mod_campaign)
                          { mod_campaign.is_active = false; });
    }
    // ... other actions ...

private:
    // Private helper functions
};

EOSIO_DISPATCH(crowdfunding, (createcamp)(contribute)(withdraw))
