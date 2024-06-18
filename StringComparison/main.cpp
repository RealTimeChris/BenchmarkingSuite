#include "BenchmarkSuite.hpp"
#include "glaze/glaze.hpp"
#include <concepts>// Required for concepts
#include "../StringComparison/jsonifier/Index.hpp"


struct search_metadata_data {
	std::string since_id_str{};
	std::string next_results{};
	std::string refresh_url{};
	std::string max_id_str{};
	double completed_in{};
	std::string query{};
	int64_t since_id{};
	int64_t count{};
	double max_id{};
};

struct hashtag {
	std::vector<int64_t> indices{};
	std::string text{};
};

struct large_data {
	std::string resize{};
	int64_t w{};
	int64_t h{};
};

struct sizes_data {
	large_data medium{};
	large_data small{};
	large_data thumb{};
	large_data large{};
};

struct media_data {
	std::optional<std::string> source_status_id_str{};
	std::optional<double> source_status_id{};
	std::vector<int64_t> indices{};
	std::string media_url_https{};
	std::string expanded_url{};
	std::string display_url{};
	std::string media_url{};
	std::string id_str{};
	std::string type{};
	sizes_data sizes{};
	std::string url{};
	double id{};
};

struct url_data {
	std::vector<int64_t> indices{};
	std::string expanded_url{};
	std::string display_url{};
	std::string url{};
};

struct user_mention {
	std::vector<int64_t> indices{};
	std::string screen_name{};
	std::string id_str{};
	std::string name{};
	int64_t id{};
};

struct status_entities {
	std::optional<std::vector<media_data>> media{};
	std::vector<user_mention> user_mentions{};
	std::vector<std::nullptr_t> symbols{};
	std::vector<hashtag> hashtags{};
	std::vector<url_data> urls{};
};

struct metadata_data {
	std::string iso_language_code{};
	std::string result_type{};
};

struct description_data {
	std::vector<url_data> urls{};
};

struct user_entities {
	std::optional<description_data> url{};
	description_data description{};
};

struct twitter_user {
	std::string profile_background_image_url_https{};
	std::optional<std::string> profile_banner_url{};
	std::string profile_sidebar_border_color{};
	std::string profile_background_image_url{};
	std::string profile_sidebar_fill_color{};
	std::optional<std::string> time_zone{};
	std::string profile_background_color{};
	std::string profile_image_url_https{};
	std::optional<int64_t> utc_offset{};
	bool profile_use_background_image{};
	std::optional<std::string> url{};
	std::string profile_text_color{};
	std::string profile_link_color{};
	std::string profile_image_url{};
	bool profile_background_tile{};
	bool is_translation_enabled{};
	bool default_profile_image{};
	bool contributors_enabled{};
	bool follow_request_sent{};
	int64_t favourites_count{};
	std::string description{};
	std::string screen_name{};
	int64_t followers_count{};
	int64_t statuses_count{};
	std::string created_at{};
	user_entities entities{};
	int64_t friends_count{};
	bool default_profile{};
	int64_t listed_count{};
	std::string location{};
	bool user_protected{};
	bool is_translator{};
	std::string id_str{};
	bool notifications{};
	std::string string{};
	std::string name{};
	bool geo_enabled{};
	std::string lang{};
	bool following{};
	bool verified{};
	int64_t id{};
};

struct retweeted_status_data {
	std::optional<std::string> in_reply_to_status_id_str{};
	std::optional<std::string> in_reply_to_user_id_str{};
	std::optional<std::string> in_reply_to_screen_name{};
	std::optional<double> in_reply_to_status_id{};
	std::optional<int64_t> in_reply_to_user_id{};
	std::optional<bool> possibly_sensitive{};
	std::nullptr_t contributors{ nullptr };
	std::nullptr_t coordinates{ nullptr };
	std::nullptr_t place{ nullptr };
	std::nullptr_t geo{ nullptr };
	status_entities entities{};
	int64_t favorite_count{};
	metadata_data metadata{};
	std::string created_at{};
	int64_t retweet_count{};
	std::string source{};
	std::string id_str{};
	twitter_user user{};
	std::string lang{};
	std::string text{};
	bool truncated{};
	bool favorited{};
	bool retweeted{};
	double id{};
};

struct status_data {
	std::optional<retweeted_status_data> retweeted_status{};
	std::optional<std::string> in_reply_to_status_id_str{};
	std::optional<std::string> in_reply_to_user_id_str{};
	std::optional<std::string> in_reply_to_screen_name{};
	std::optional<double> in_reply_to_status_id{};
	std::optional<int64_t> in_reply_to_user_id{};
	std::optional<bool> possibly_sensitive{};
	std::nullptr_t contributors{ nullptr };
	std::nullptr_t coordinates{ nullptr };
	std::nullptr_t place{ nullptr };
	std::nullptr_t geo{ nullptr };
	status_entities entities{};
	int64_t favorite_count{};
	metadata_data metadata{};
	std::string created_at{};
	int64_t retweet_count{};
	std::string source{};
	std::string id_str{};
	twitter_user user{};
	std::string lang{};
	std::string text{};
	bool truncated{};
	bool favorited{};
	bool retweeted{};
	double id{};
};

struct twitter_message {
	search_metadata_data search_metadata{};
	std::vector<status_data> statuses{};
};

struct icon_emoji_data {
	std::optional<std::string> name{};
	std::nullptr_t id{ nullptr };
};

struct permission_overwrite {
	std::string allow{};
	std::string deny{};
	std::string id{};
	int64_t type{};
};

struct channel_data {
	std::vector<permission_overwrite> permission_overwrites{};
	std::optional<std::string> last_message_id{};
	int64_t default_thread_rate_limit_per_user{};
	std::vector<std::nullptr_t> applied_tags{};
	std::vector<std::nullptr_t> recipients{};
	int64_t default_auto_archive_duration{};
	std::nullptr_t rtc_region{ nullptr };
	std::nullptr_t status{ nullptr };
	std::string last_pin_timestamp{};
	std::nullptr_t topic{ nullptr };
	int64_t rate_limit_per_user{};
	icon_emoji_data icon_emoji{};
	int64_t total_message_sent{};
	int64_t video_quality_mode{};
	std::string application_id{};
	std::string permissions{};
	int64_t message_count{};
	std::string parent_id{};
	int64_t member_count{};
	std::string owner_id{};
	std::string guild_id{};
	int64_t user_limit{};
	int64_t position{};
	std::string name{};
	std::string icon{};
	int64_t version{};
	int64_t bitrate{};
	std::string id{};
	int64_t flags{};
	int64_t type{};
	bool managed{};
	bool nsfw{};
};

struct user_data {
	std::nullptr_t avatar_decoration_data{ nullptr };
	std::optional<std::string> display_name{};
	std::optional<std::string> global_name{};
	std::optional<std::string> avatar{};
	std::nullptr_t banner{ nullptr };
	std::nullptr_t locale{ nullptr };
	std::string discriminator{};
	std::string user_name{};
	int64_t accent_color{};
	int64_t premium_type{};
	int64_t public_flags{};
	std::string email{};
	bool mfa_enabled{};
	std::string id{};
	int64_t flags{};
	bool verified{};
	bool system{};
	bool bot{};
};

struct member_data {
	std::nullptr_t communication_disabled_until{ nullptr };
	std::nullptr_t premium_since{ nullptr };
	std::optional<std::string> nick{};
	std::nullptr_t avatar{ nullptr };
	std::vector<std::string> roles{};
	std::string permissions{};
	std::string joined_at{};
	std::string guild_id{};
	user_data user{};
	int64_t flags{};
	bool pending{};
	bool deaf{};
	bool mute{};
};

struct tags_data {
	std::nullptr_t premium_subscriber{ nullptr };
	std::optional<std::string> bot_id{};
};

struct role_data {
	std::nullptr_t unicode_emoji{ nullptr };
	std::nullptr_t icon{ nullptr };
	std::string permissions{};
	int64_t position{};
	std::string name{};
	bool mentionable{};
	int64_t version{};
	std::string id{};
	tags_data tags{};
	int64_t color{};
	int64_t flags{};
	bool managed{};
	bool hoist{};
};

struct guild_data {
	std::nullptr_t latest_on_boarding_question_id{ nullptr };
	std::vector<std::nullptr_t> guild_scheduled_events{};
	std::nullptr_t safety_alerts_channel_id{ nullptr };
	std::nullptr_t inventory_settings{ nullptr };
	std::vector<std::nullptr_t> voice_states{};
	std::nullptr_t discovery_splash{ nullptr };
	std::nullptr_t vanity_url_code{ nullptr };
	std::nullptr_t application_id{ nullptr };
	std::nullptr_t afk_channel_id{ nullptr };
	int64_t default_message_notifications{};
	int64_t max_stage_video_channel_users{};
	std::string public_updates_channel_id{};
	std::nullptr_t description{ nullptr };
	std::vector<std::nullptr_t> threads{};
	std::vector<channel_data> channels{};
	int64_t premium_subscription_count{};
	int64_t approximate_presence_count{};
	std::vector<std::string> features{};
	std::vector<std::string> stickers{};
	bool premium_progress_bar_enabled{};
	std::vector<member_data> members{};
	std::nullptr_t hub_type{ nullptr };
	int64_t approximate_member_count{};
	int64_t explicit_content_filter{};
	int64_t max_video_channel_users{};
	std::nullptr_t splash{ nullptr };
	std::nullptr_t banner{ nullptr };
	std::string system_channel_id{};
	std::string widget_channel_id{};
	std::string preferred_locale{};
	int64_t system_channel_flags{};
	std::string rules_channel_id{};
	std::vector<role_data> roles{};
	int64_t verification_level{};
	std::string permissions{};
	int64_t max_presences{};
	std::string discovery{};
	std::string joined_at{};
	int64_t member_count{};
	int64_t premium_tier{};
	std::string owner_id{};
	int64_t max_members{};
	int64_t afk_timeout{};
	bool widget_enabled{};
	std::string region{};
	int64_t nsfw_level{};
	int64_t mfa_level{};
	std::string name{};
	std::string icon{};
	bool unavailable{};
	std::string id{};
	int64_t flags{};
	bool large{};
	bool owner{};
	bool nsfw{};
	bool lazy{};
};

struct discord_message {
	std::string t{};
	guild_data d{};
	int64_t op{};
	int64_t s{};
};

template<> struct jsonifier::core<icon_emoji_data> {
	using value_type				 = icon_emoji_data;
	static constexpr auto parseValue = createValue<&value_type::name, &value_type::id>();
};

template<> struct jsonifier::core<permission_overwrite> {
	using value_type				 = permission_overwrite;
	static constexpr auto parseValue = createValue<&value_type::allow, &value_type::deny, &value_type::id, &value_type::type>();
};

template<> struct jsonifier::core<channel_data> {
	using value_type				 = channel_data;
	static constexpr auto parseValue = createValue<&value_type::permission_overwrites, &value_type::last_message_id, &value_type::default_thread_rate_limit_per_user,
		&value_type::applied_tags, &value_type::recipients, &value_type::default_auto_archive_duration, &value_type::rtc_region, &value_type::status,
		&value_type::last_pin_timestamp, &value_type::topic, &value_type::rate_limit_per_user, &value_type::icon_emoji, &value_type::total_message_sent,
		&value_type::video_quality_mode, &value_type::application_id, &value_type::permissions>();
};

template<> struct jsonifier::core<user_data> {
	using value_type = user_data;
	static constexpr auto parseValue =
		createValue<&value_type::avatar_decoration_data, &value_type::display_name, &value_type::global_name, &value_type::avatar, &value_type::banner, &value_type::locale,
			&value_type::discriminator, &value_type::user_name, &value_type::accent_color, &value_type::premium_type, &value_type::public_flags, &value_type::email,
			&value_type::mfa_enabled, &value_type::id, &value_type::flags, &value_type::verified, &value_type::system, &value_type::bot>();
};

template<> struct jsonifier::core<member_data> {
	using value_type = member_data;
	static constexpr auto parseValue =
		createValue<&value_type::communication_disabled_until, &value_type::premium_since, &value_type::nick, &value_type::avatar, &value_type::roles, &value_type::permissions,
			&value_type::joined_at, &value_type::guild_id, &value_type::user, &value_type::flags, &value_type::pending, &value_type::deaf, &value_type::mute>();
};

template<> struct jsonifier::core<tags_data> {
	using value_type				 = tags_data;
	static constexpr auto parseValue = createValue<&value_type::premium_subscriber, &value_type::bot_id>();
};

template<> struct jsonifier::core<role_data> {
	using value_type				 = role_data;
	static constexpr auto parseValue = createValue<&value_type::unicode_emoji, &value_type::icon, &value_type::permissions, &value_type::position, &value_type::name,
		&value_type::mentionable, &value_type::version, &value_type::id, &value_type::tags, &value_type::color, &value_type::flags, &value_type::managed, &value_type::hoist>();
};

template<> struct jsonifier::core<guild_data> {
	using value_type				 = guild_data;
	static constexpr auto parseValue = createValue<&value_type::latest_on_boarding_question_id, &value_type::guild_scheduled_events, &value_type::safety_alerts_channel_id,
		&value_type::inventory_settings, &value_type::voice_states, &value_type::discovery_splash, &value_type::vanity_url_code, &value_type::application_id,
		&value_type::afk_channel_id, &value_type::default_message_notifications, &value_type::max_stage_video_channel_users, &value_type::public_updates_channel_id,
		&value_type::description, &value_type::threads, &value_type::channels, &value_type::premium_subscription_count, &value_type::approximate_presence_count,
		&value_type::features, &value_type::stickers, &value_type::premium_progress_bar_enabled, &value_type::members, &value_type::hub_type, &value_type::approximate_member_count,
		&value_type::explicit_content_filter, &value_type::max_video_channel_users, &value_type::splash, &value_type::banner, &value_type::system_channel_id,
		&value_type::widget_channel_id, &value_type::preferred_locale, &value_type::system_channel_flags, &value_type::rules_channel_id, &value_type::roles,
		&value_type::verification_level, &value_type::permissions, &value_type::max_presences, &value_type::discovery, &value_type::joined_at, &value_type::member_count,
		&value_type::premium_tier, &value_type::owner_id, &value_type::max_members, &value_type::afk_timeout, &value_type::widget_enabled, &value_type::region,
		&value_type::nsfw_level, &value_type::mfa_level, &value_type::name, &value_type::icon, &value_type::unavailable, &value_type::id, &value_type::flags, &value_type::large,
		&value_type::owner, &value_type::nsfw, &value_type::lazy>();
};

template<> struct jsonifier::core<discord_message> {
	using value_type				 = discord_message;
	static constexpr auto parseValue = createValue<&value_type::t, &value_type::d, &value_type::op, &value_type::s>();
};

template<> struct glz::meta<icon_emoji_data> {
	using value_type			= icon_emoji_data;
	static constexpr auto value = object("name", &value_type::name, "id", &value_type::id);
};

template<> struct glz::meta<permission_overwrite> {
	using value_type			= permission_overwrite;
	static constexpr auto value = object("allow", &value_type::allow, "deny", &value_type::deny, "id", &value_type::id, "type", &value_type::type);
};

template<> struct glz::meta<channel_data> {
	using value_type			= channel_data;
	static constexpr auto value = object("permission_overwrites", &value_type::permission_overwrites, "last_message_id", &value_type::last_message_id,
		"default_thread_rate_limit_per_user", &value_type::default_thread_rate_limit_per_user, "applied_tags", &value_type::applied_tags, "recipients", &value_type::recipients,
		"default_auto_archive_duration", &value_type::default_auto_archive_duration, "rtc_region", &value_type::rtc_region, "status", &value_type::status, "last_pin_timestamp",
		&value_type::last_pin_timestamp, "topic", &value_type::topic, "rate_limit_per_user", &value_type::rate_limit_per_user, "icon_emoji", &value_type::icon_emoji,
		"total_message_sent", &value_type::total_message_sent, "video_quality_mode", &value_type::video_quality_mode, "application_id", &value_type::application_id, "permissions",
		&value_type::permissions, "message_count", &value_type::message_count, "parent_id", &value_type::parent_id, "member_count", &value_type::member_count, "owner_id",
		&value_type::owner_id, "guild_id", &value_type::guild_id, "user_limit", &value_type::user_limit, "position", &value_type::position, "name", &value_type::name, "icon",
		&value_type::icon, "version", &value_type::version, "bitrate", &value_type::bitrate, "id", &value_type::id, "flags", &value_type::flags, "type", &value_type::type,
		"managed", &value_type::managed, "nsfw", &value_type::nsfw);
};

template<> struct glz::meta<user_data> {
	using value_type			= user_data;
	static constexpr auto value = object("avatar_decoration_data", &value_type::avatar_decoration_data, "display_name", &value_type::display_name, "global_name",
		&value_type::global_name, "avatar", &value_type::avatar, "banner", &value_type::banner, "locale", &value_type::locale, "discriminator", &value_type::discriminator,
		"user_name", &value_type::user_name, "accent_color", &value_type::accent_color, "premium_type", &value_type::premium_type, "public_flags", &value_type::public_flags,
		"email", &value_type::email, "mfa_enabled", &value_type::mfa_enabled, "id", &value_type::id, "flags", &value_type::flags, "verified", &value_type::verified, "system",
		&value_type::system, "bot", &value_type::bot);
};

template<> struct glz::meta<member_data> {
	using value_type			= member_data;
	static constexpr auto value = object("communication_disabled_until", &value_type::communication_disabled_until, "premium_since", &value_type::premium_since, "nick",
		&value_type::nick, "avatar", &value_type::avatar, "roles", &value_type::roles, "permissions", &value_type::permissions, "joined_at", &value_type::joined_at, "guild_id",
		&value_type::guild_id, "user", &value_type::user, "flags", &value_type::flags, "pending", &value_type::pending, "deaf", &value_type::deaf, "mute", &value_type::mute);
};

template<> struct glz::meta<tags_data> {
	using value_type			= tags_data;
	static constexpr auto value = object("premium_subscriber", &value_type::premium_subscriber, "bot_id", &value_type::bot_id);
};

template<> struct glz::meta<role_data> {
	using value_type			= role_data;
	static constexpr auto value = object("unicode_emoji", &value_type::unicode_emoji, "icon", &value_type::icon, "permissions", &value_type::permissions, "position",
		&value_type::position, "name", &value_type::name, "mentionable", &value_type::mentionable, "version", &value_type::version, "id", &value_type::id, "tags",
		&value_type::tags, "color", &value_type::color, "flags", &value_type::flags, "managed", &value_type::managed, "hoist", &value_type::hoist);
};

template<> struct glz::meta<guild_data> {
	using value_type			= guild_data;
	static constexpr auto value = object("latest_on_boarding_question_id", &value_type::latest_on_boarding_question_id, "guild_scheduled_events",
		&value_type::guild_scheduled_events, "safety_alerts_channel_id", &value_type::safety_alerts_channel_id, "inventory_settings", &value_type::inventory_settings,
		"voice_states", &value_type::voice_states, "discovery_splash", &value_type::discovery_splash, "vanity_url_code", &value_type::vanity_url_code, "application_id",
		&value_type::application_id, "afk_channel_id", &value_type::afk_channel_id, "default_message_notifications", &value_type::default_message_notifications,
		"max_stage_video_channel_users", &value_type::max_stage_video_channel_users, "public_updates_channel_id", &value_type::public_updates_channel_id, "description",
		&value_type::description, "threads", &value_type::threads, "channels", &value_type::channels, "premium_subscription_count", &value_type::premium_subscription_count,
		"approximate_presence_count", &value_type::approximate_presence_count, "features", &value_type::features, "stickers", &value_type::stickers, "premium_progress_bar_enabled",
		&value_type::premium_progress_bar_enabled, "members", &value_type::members, "hub_type", &value_type::hub_type, "approximate_member_count",
		&value_type::approximate_member_count, "explicit_content_filter", &value_type::explicit_content_filter, "max_video_channel_users", &value_type::max_video_channel_users,
		"splash", &value_type::splash, "banner", &value_type::banner, "system_channel_id", &value_type::system_channel_id, "widget_channel_id", &value_type::widget_channel_id,
		"preferred_locale", &value_type::preferred_locale, "system_channel_flags", &value_type::system_channel_flags, "rules_channel_id", &value_type::rules_channel_id, "roles",
		&value_type::roles, "verification_level", &value_type::verification_level, "permissions", &value_type::permissions, "max_presences", &value_type::max_presences,
		"discovery", &value_type::discovery, "joined_at", &value_type::joined_at, "member_count", &value_type::member_count, "premium_tier", &value_type::premium_tier, "owner_id",
		&value_type::owner_id, "max_members", &value_type::max_members, "afk_timeout", &value_type::afk_timeout, "widget_enabled", &value_type::widget_enabled, "region",
		&value_type::region, "nsfw_level", &value_type::nsfw_level, "mfa_level", &value_type::mfa_level, "name", &value_type::name, "icon", &value_type::icon, "unavailable",
		&value_type::unavailable, "id", &value_type::id, "flags", &value_type::flags, "large", &value_type::large, "owner", &value_type::owner, "nsfw", &value_type::nsfw, "lazy");
};

template<> struct jsonifier::core<search_metadata_data> {
	using value_type				 = search_metadata_data;
	static constexpr auto parseValue = createValue<&value_type::since_id_str, &value_type::next_results, &value_type::refresh_url, &value_type::max_id_str,
		&value_type::completed_in, &value_type::query, &value_type::since_id, &value_type::count, &value_type::max_id>();
};

template<> struct jsonifier::core<hashtag> {
	using value_type				 = hashtag;
	static constexpr auto parseValue = createValue<&value_type::indices, &value_type::text>();
};

template<> struct jsonifier::core<large_data> {
	using value_type				 = large_data;
	static constexpr auto parseValue = createValue<&value_type::resize, &value_type::w, &value_type::h>();
};

template<> struct jsonifier::core<sizes_data> {
	using value_type				 = sizes_data;
	static constexpr auto parseValue = createValue<&value_type::medium, &value_type::small, &value_type::thumb, &value_type::large>();
};

template<> struct jsonifier::core<media_data> {
	using value_type = media_data;
	static constexpr auto parseValue =
		createValue<&value_type::source_status_id_str, &value_type::source_status_id, &value_type::indices, &value_type::media_url_https, &value_type::expanded_url,
			&value_type::display_url, &value_type::media_url, &value_type::id_str, &value_type::type, &value_type::sizes, &value_type::url, &value_type::id>();
};

template<> struct jsonifier::core<url_data> {
	using value_type				 = url_data;
	static constexpr auto parseValue = createValue<&value_type::indices, &value_type::expanded_url, &value_type::display_url, &value_type::url>();
};

template<> struct jsonifier::core<user_mention> {
	using value_type				 = user_mention;
	static constexpr auto parseValue = createValue<&value_type::indices, &value_type::screen_name, &value_type::id_str, &value_type::name, &value_type::id>();
};

template<> struct jsonifier::core<status_entities> {
	using value_type				 = status_entities;
	static constexpr auto parseValue = createValue<&value_type::media, &value_type::user_mentions, &value_type::symbols, &value_type::hashtags, &value_type::urls>();
};

template<> struct jsonifier::core<metadata_data> {
	using value_type				 = metadata_data;
	static constexpr auto parseValue = createValue<&value_type::iso_language_code, &value_type::result_type>();
};

template<> struct jsonifier::core<description_data> {
	using value_type				 = description_data;
	static constexpr auto parseValue = createValue<&value_type::urls>();
};

template<> struct jsonifier::core<user_entities> {
	using value_type				 = user_entities;
	static constexpr auto parseValue = createValue<&value_type::url, &value_type::description>();
};

template<> struct jsonifier::core<twitter_user> {
	using value_type				 = twitter_user;
	static constexpr auto parseValue = createValue<&value_type::profile_background_image_url_https, &value_type::profile_banner_url, &value_type::profile_background_image_url,
		&value_type::profile_sidebar_border_color, &value_type::profile_sidebar_fill_color, &value_type::time_zone, &value_type::profile_background_color,
		&value_type::profile_image_url_https, &value_type::utc_offset, &value_type::profile_use_background_image, &value_type::url, &value_type::profile_text_color,
		&value_type::profile_link_color, &value_type::profile_image_url, &value_type::profile_background_tile, &value_type::is_translation_enabled,
		&value_type::default_profile_image, &value_type::contributors_enabled, &value_type::follow_request_sent, &value_type::favourites_count, &value_type::description,
		&value_type::screen_name, &value_type::followers_count, &value_type::statuses_count, &value_type::created_at, &value_type::entities, &value_type::friends_count,
		&value_type::default_profile, &value_type::listed_count, &value_type::location, &value_type::user_protected, &value_type::is_translator, &value_type::id_str,
		&value_type::notifications, &value_type::string, &value_type::name, &value_type::geo_enabled, &value_type::lang, &value_type::following, &value_type::verified,
		&value_type::id>();
};

template<> struct jsonifier::core<retweeted_status_data> {
	using value_type				 = retweeted_status_data;
	static constexpr auto parseValue = createValue<&value_type::in_reply_to_status_id_str, &value_type::in_reply_to_user_id_str, &value_type::in_reply_to_screen_name,
		&value_type::in_reply_to_status_id, &value_type::in_reply_to_user_id, &value_type::possibly_sensitive, &value_type::contributors, &value_type::coordinates,
		&value_type::place, &value_type::geo, &value_type::entities, &value_type::favorite_count, &value_type::metadata, &value_type::created_at, &value_type::retweet_count,
		&value_type::source, &value_type::id_str, &value_type::user, &value_type::lang, &value_type::text, &value_type::truncated, &value_type::favorited, &value_type::retweeted,
		&value_type::id>();
};

template<> struct jsonifier::core<status_data> {
	using value_type				 = status_data;
	static constexpr auto parseValue = createValue<&value_type::in_reply_to_status_id_str, &value_type::in_reply_to_user_id_str, &value_type::in_reply_to_screen_name,
		&value_type::in_reply_to_status_id, &value_type::in_reply_to_user_id, &value_type::possibly_sensitive, &value_type::contributors, &value_type::coordinates,
		&value_type::retweeted_status, &value_type::place, &value_type::geo, &value_type::entities, &value_type::favorite_count, &value_type::metadata, &value_type::created_at,
		&value_type::retweet_count, &value_type::source, &value_type::id_str, &value_type::user, &value_type::lang, &value_type::text, &value_type::truncated,
		&value_type::favorited, &value_type::retweeted, &value_type::id>();
};

template<> struct jsonifier::core<twitter_message> {
	using value_type				 = twitter_message;
	static constexpr auto parseValue = createValue<&value_type::search_metadata, &value_type::statuses>();
};

template<> struct glz::meta<search_metadata_data> {
	using value_type = search_metadata_data;
	static constexpr auto value =
		object("since_id_str", &value_type::since_id_str, "next_results", &value_type::next_results, "refresh_url", &value_type::refresh_url, "max_id_str", &value_type::max_id_str,
			"completed_in", &value_type::completed_in, "query", &value_type::query, "since_id", &value_type::since_id, "count", &value_type::count, "max_id", &value_type::max_id);
};

template<> struct glz::meta<hashtag> {
	using value_type			= hashtag;
	static constexpr auto value = object("indices", &value_type::indices, "text", &value_type::text);
};

template<> struct glz::meta<large_data> {
	using value_type			= large_data;
	static constexpr auto value = object("resize", &value_type::resize, "w", &value_type::w, "h", &value_type::h);
};

template<> struct glz::meta<sizes_data> {
	using value_type			= sizes_data;
	static constexpr auto value = object("medium", &value_type::medium, "small", &value_type::small, "thumb", &value_type::thumb, "large", &value_type::large);
};

template<> struct glz::meta<media_data> {
	using value_type			= media_data;
	static constexpr auto value = object("source_status_id_str", &value_type::source_status_id_str, "source_status_id", &value_type::source_status_id, "indices",
		&value_type::indices, "media_url_https", &value_type::media_url_https, "expanded_url", &value_type::expanded_url, "display_url", &value_type::display_url, "media_url",
		&value_type::media_url, "id_str", &value_type::id_str, "type", &value_type::type, "sizes", &value_type::sizes, "url", &value_type::url, "id", &value_type::id);
};

template<> struct glz::meta<url_data> {
	using value_type = url_data;
	static constexpr auto value =
		object("indices", &value_type::indices, "expanded_url", &value_type::expanded_url, "display_url", &value_type::display_url, "url", &value_type::url);
};

template<> struct glz::meta<user_mention> {
	using value_type = user_mention;
	static constexpr auto value =
		object("indices", &value_type::indices, "screen_name", &value_type::screen_name, "id_str", &value_type::id_str, "name", &value_type::name, "id", &value_type::id);
};

template<> struct glz::meta<status_entities> {
	using value_type			= status_entities;
	static constexpr auto value = object("media", &value_type::media, "user_mentions", &value_type::user_mentions, "symbols", &value_type::symbols, "hashtags",
		&value_type::hashtags, "urls", &value_type::urls);
};

template<> struct glz::meta<metadata_data> {
	using value_type			= metadata_data;
	static constexpr auto value = object("iso_language_code", &value_type::iso_language_code, "result_type", &value_type::result_type);
};

template<> struct glz::meta<description_data> {
	using value_type			= description_data;
	static constexpr auto value = object("urls", &value_type::urls);
};

template<> struct glz::meta<user_entities> {
	using value_type			= user_entities;
	static constexpr auto value = object("url", &value_type::url, "description", &value_type::description);
};

template<> struct glz::meta<twitter_user> {
	using value_type			= twitter_user;
	static constexpr auto value = object("profile_background_image_url_https", &value_type::profile_background_image_url_https, "profile_banner_url",
		&value_type::profile_banner_url, "profile_background_image_url", &value_type::profile_banner_url, "profile_sidebar_border_color", &value_type::profile_sidebar_border_color,
		"profile_sidebar_fill_color", &value_type::profile_sidebar_fill_color, "time_zone", &value_type::time_zone, "profile_background_color",
		&value_type::profile_background_color, "profile_image_url_https", &value_type::profile_image_url_https, "utc_offset", &value_type::utc_offset,
		"profile_use_background_image", &value_type::profile_use_background_image, "url", &value_type::url, "profile_text_color", &value_type::profile_text_color,
		"profile_link_color", &value_type::profile_link_color, "profile_image_url", &value_type::profile_image_url, "profile_background_tile", &value_type::profile_background_tile,
		"is_translation_enabled", &value_type::is_translation_enabled, "default_profile_image", &value_type::default_profile_image, "contributors_enabled",
		&value_type::contributors_enabled, "follow_request_sent", &value_type::follow_request_sent, "favourites_count", &value_type::favourites_count, "description",
		&value_type::description, "screen_name", &value_type::screen_name, "followers_count", &value_type::followers_count, "statuses_count", &value_type::statuses_count,
		"created_at", &value_type::created_at, "entities", &value_type::entities, "friends_count", &value_type::friends_count, "default_profile", &value_type::default_profile,
		"listed_count", &value_type::listed_count, "location", &value_type::location, "user_protected", &value_type::user_protected, "is_translator", &value_type::is_translator,
		"id_str", &value_type::id_str, "notifications", &value_type::notifications, "string", &value_type::string, "name", &value_type::name, "geo_enabled",
		&value_type::geo_enabled, "lang", &value_type::lang, "following", &value_type::following, "verified", &value_type::verified, "id", &value_type::id);
};

template<> struct glz::meta<retweeted_status_data> {
	using value_type			= retweeted_status_data;
	static constexpr auto value = object("in_reply_to_status_id_str", &value_type::in_reply_to_status_id_str, "in_reply_to_user_id_str", &value_type::in_reply_to_user_id_str,
		"in_reply_to_screen_name", &value_type::in_reply_to_screen_name, "in_reply_to_status_id", &value_type::in_reply_to_status_id, "in_reply_to_user_id",
		&value_type::in_reply_to_user_id, "possibly_sensitive", &value_type::possibly_sensitive, "contributors", &value_type::contributors, "coordinates", &value_type::coordinates,
		"place", &value_type::place, "geo", &value_type::geo, "entities", &value_type::entities, "favorite_count", &value_type::favorite_count, "metadata", &value_type::metadata,
		"created_at", &value_type::created_at, "retweet_count", &value_type::retweet_count, "source", &value_type::source, "id_str", &value_type::id_str, "user", &value_type::user,
		"lang", &value_type::lang, "text", &value_type::text, "truncated", &value_type::truncated, "favorited", &value_type::favorited, "retweeted", &value_type::retweeted, "id",
		&value_type::id);
};

template<> struct glz::meta<status_data> {
	using value_type			= status_data;
	static constexpr auto value = object("in_reply_to_status_id_str", &value_type::in_reply_to_status_id_str, "in_reply_to_user_id_str", &value_type::in_reply_to_user_id_str,
		"in_reply_to_screen_name", &value_type::in_reply_to_screen_name, "in_reply_to_status_id", &value_type::in_reply_to_status_id, "in_reply_to_user_id",
		&value_type::in_reply_to_user_id, "possibly_sensitive", &value_type::possibly_sensitive, "contributors", &value_type::contributors, "coordinates", &value_type::coordinates,
		"place", &value_type::place, "geo", &value_type::geo, "entities", &value_type::entities, "favorite_count", &value_type::favorite_count, "metadata", &value_type::metadata,
		"created_at", &value_type::created_at, "retweeted_status", &value_type::retweeted_status, "retweet_count", &value_type::retweet_count, "source", &value_type::source,
		"id_str", &value_type::id_str, "user", &value_type::user, "lang", &value_type::lang, "text", &value_type::text, "truncated", &value_type::truncated, "favorited",
		&value_type::favorited, "retweeted", &value_type::retweeted, "id", &value_type::id);
};

template<> struct glz::meta<twitter_message> {
	using value_type			= twitter_message;
	static constexpr auto value = object("search_metadata", &value_type::search_metadata, "statuses", &value_type::statuses);
};
 
#include <random>

inline static std::random_device randomEngine{};
inline static std::mt19937_64 gen{ randomEngine() };

static constexpr std::string_view charset{ "!#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~\"\\\r\b\f\t\n" };

static uint64_t randomizeNumberUniform(uint64_t lowEnd, uint64_t highEnd) {
	std::uniform_int_distribution<uint64_t> dis(lowEnd, highEnd);
	return dis(randomEngine);
}

static std::string generateString() {
	uint64_t length = randomizeNumberUniform(64, 128);
	std::string result{};
	for (int32_t x = 0; x < length; ++x) {
		result += charset[randomizeNumberUniform(0, charset.size())];
	}
	return result;
}

template<uint64_t length> struct convert_length_to_int {
	static_assert(length <= 8, "Sorry, but that string is too long!");
	using type = std::conditional_t<length == 1, uint8_t,
		std::conditional_t<length <= 2, uint16_t, std::conditional_t<length <= 4, uint32_t, std::conditional_t<length <= 8, uint64_t, void>>>>;
};

template<uint64_t length> using convert_length_to_int_t = typename convert_length_to_int<length>::type;

template<bnch_swt::string_literal string> constexpr convert_length_to_int_t<string.size()> getStringAsInt() {
	const char* stringNew = string.data();
	convert_length_to_int_t<string.size()> returnValue{};
	for (uint64_t x = 0; x < string.size(); ++x) {
		returnValue |= static_cast<convert_length_to_int_t<string.size()>>(stringNew[x]) << x * 8;
	}
	return returnValue;
}

template<bnch_swt::string_literal string> JSONIFIER_INLINE bool compareStringAsInt(const char* iter) {
	static constexpr auto newString{ getStringAsInt<string>() };
	convert_length_to_int_t<string.size()> newString02{};
	std::memcpy(&newString02, iter, string.size());
	return newString == newString02;
}

template<typename iterator_type, jsonifier::concepts::bool_t bool_type> JSONIFIER_INLINE bool parseBool(bool_type& value, iterator_type&& iter) {
	if (compareStringAsInt<"true">(iter)) {
		value = true;
		iter += 4;
		return true;
	} else if (compareStringAsInt<"false">(iter)) {
		value = false;
		iter += 5;
		return true;
	} else {
		return false;
	}
}

template<typename iterator_type, jsonifier::concepts::bool_t bool_type> JSONIFIER_INLINE bool parseBool02(bool_type& value, iterator_type&& iter) {
	uint64_t c{};
	// Note that because our buffer must be null terminated, we can read one more index without checking:
	std::memcpy(&c, iter, 5);
	constexpr uint64_t u_true  = 0b00000000'00000000'00000000'00000000'01100101'01110101'01110010'01110100;
	constexpr uint64_t u_false = 0b00000000'00000000'00000000'01100101'01110011'01101100'01100001'01100110;
	// We have to wipe the 5th character for true testing
	if ((c & 0xFF'FF'FF'00'FF'FF'FF'FF) == u_true) {
		value = true;
		iter += 4;
	} else {
		if (c != u_false) [[unlikely]] {
			return false;
		}
		value = false;
		iter += 5;
	}
	return true;
}

template<jsonifier_internal::simd_structural_iterator_t iterator_type, jsonifier::concepts::bool_t bool_type>
JSONIFIER_INLINE bool parseBool(bool_type& value, iterator_type&& iter) {
	if (compareStringAsInt<"true">(iter)) {
		value = true;
		++iter;
		return true;
	} else if (compareStringAsInt<"false">(iter)) {
		value = false;
		++iter;
		return true;
	} else {
		return false;
	}
}

template<typename iterator_type> JSONIFIER_INLINE bool parseNull(iterator_type&& iter) {
	if (compareStringAsInt<"null">(iter)) {
		iter += 4;
		return true;
	} else {
		return false;
	}
}

template<typename iterator_type> JSONIFIER_INLINE bool parseNull02(iterator_type&& iter) {
	static constexpr bnch_swt::string_literal str{ "null" };
	if (!glz::compare<str.size()>(iter, str.values)) [[unlikely]] {
		return false;
	} else [[likely]] {
		iter += str.size();
		return true;
	}
}

template<typename value_type, uint64_t size> constexpr auto convertRawToArray(value_type values[size]) {
	std::array<value_type, size> returnValues{};
	for (uint64_t x = 0; x < size; ++x) {
		returnValues[x] = values[x];
	}
	return returnValues;
}

// Function prototypes
void fillEmptyStrings(twitter_message& message);
void fillEmptyStrings(search_metadata_data& data);
void fillEmptyStrings(status_data& status);
void fillEmptyStrings(retweeted_status_data& retweetedStatus);
void fillEmptyStrings(twitter_user& user);
void fillEmptyStrings(user_entities& entities);
void fillEmptyStrings(description_data& description);
void fillEmptyStrings(metadata_data& metadata);
void fillEmptyStrings(status_entities& entities);
void fillEmptyStrings(media_data& media);
void fillEmptyStrings(sizes_data& sizes);
void fillEmptyStrings(large_data& large);
void fillEmptyStrings(url_data& url);
void fillEmptyStrings(user_mention& mention);
void fillEmptyStrings(hashtag& tag);

// Recursive function to fill empty strings in twitter_message
void fillEmptyStrings(twitter_message& message) {
	fillEmptyStrings(message.search_metadata);
	for (auto& status: message.statuses) {
		fillEmptyStrings(status);
	}
}

void fillEmptyStrings(search_metadata_data& data) {
	if (data.since_id_str.empty())
		data.since_id_str = generateString();
	if (data.next_results.empty())
		data.next_results = generateString();
	if (data.refresh_url.empty())
		data.refresh_url = generateString();
	if (data.max_id_str.empty())
		data.max_id_str = generateString();
	if (data.query.empty())
		data.query = generateString();
}

void fillEmptyStrings(status_data& status) {
	if (status.in_reply_to_status_id_str && status.in_reply_to_status_id_str->empty())
		status.in_reply_to_status_id_str = generateString();
	if (status.in_reply_to_user_id_str && status.in_reply_to_user_id_str->empty())
		status.in_reply_to_user_id_str = generateString();
	if (status.in_reply_to_screen_name && status.in_reply_to_screen_name->empty())
		status.in_reply_to_screen_name = generateString();
	if (status.source.empty())
		status.source = generateString();
	if (status.id_str.empty())
		status.id_str = generateString();
	if (status.created_at.empty())
		status.created_at = generateString();
	if (status.lang.empty())
		status.lang = generateString();
	if (status.text.empty())
		status.text = generateString();

	fillEmptyStrings(status.entities);
	fillEmptyStrings(status.metadata);
	fillEmptyStrings(status.user);
	if (status.retweeted_status) {
		fillEmptyStrings(*status.retweeted_status);
	}
}

void fillEmptyStrings(retweeted_status_data& retweetedStatus) {
	if (retweetedStatus.in_reply_to_status_id_str && retweetedStatus.in_reply_to_status_id_str->empty())
		retweetedStatus.in_reply_to_status_id_str = generateString();
	if (retweetedStatus.in_reply_to_user_id_str && retweetedStatus.in_reply_to_user_id_str->empty())
		retweetedStatus.in_reply_to_user_id_str = generateString();
	if (retweetedStatus.in_reply_to_screen_name && retweetedStatus.in_reply_to_screen_name->empty())
		retweetedStatus.in_reply_to_screen_name = generateString();
	if (retweetedStatus.source.empty())
		retweetedStatus.source = generateString();
	if (retweetedStatus.id_str.empty())
		retweetedStatus.id_str = generateString();
	if (retweetedStatus.created_at.empty())
		retweetedStatus.created_at = generateString();
	if (retweetedStatus.lang.empty())
		retweetedStatus.lang = generateString();
	if (retweetedStatus.text.empty())
		retweetedStatus.text = generateString();

	fillEmptyStrings(retweetedStatus.entities);
	fillEmptyStrings(retweetedStatus.metadata);
	fillEmptyStrings(retweetedStatus.user);
}

void fillEmptyStrings(twitter_user& user) {
	if (user.profile_background_image_url_https.empty())
		user.profile_background_image_url_https = generateString();
	if (user.profile_banner_url && user.profile_banner_url->empty())
		user.profile_banner_url = generateString();
	if (user.profile_sidebar_border_color.empty())
		user.profile_sidebar_border_color = generateString();
	if (user.profile_background_image_url.empty())
		user.profile_background_image_url = generateString();
	if (user.profile_sidebar_fill_color.empty())
		user.profile_sidebar_fill_color = generateString();
	if (user.time_zone && user.time_zone->empty())
		user.time_zone = generateString();
	if (user.profile_background_color.empty())
		user.profile_background_color = generateString();
	if (user.profile_image_url_https.empty())
		user.profile_image_url_https = generateString();
	if (user.url && user.url->empty())
		user.url = generateString();
	if (user.profile_text_color.empty())
		user.profile_text_color = generateString();
	if (user.profile_link_color.empty())
		user.profile_link_color = generateString();
	if (user.profile_image_url.empty())
		user.profile_image_url = generateString();
	if (user.description.empty())
		user.description = generateString();
	if (user.screen_name.empty())
		user.screen_name = generateString();
	if (user.created_at.empty())
		user.created_at = generateString();
	if (user.location.empty())
		user.location = generateString();
	if (user.id_str.empty())
		user.id_str = generateString();
	if (user.string.empty())
		user.string = generateString();
	if (user.name.empty())
		user.name = generateString();
	if (user.lang.empty())
		user.lang = generateString();

	fillEmptyStrings(user.entities);
}

void fillEmptyStrings(user_entities& entities) {
	if (entities.url) {
		fillEmptyStrings(*entities.url);
	}
	fillEmptyStrings(entities.description);
}

void fillEmptyStrings(description_data& description) {
	for (auto& url: description.urls) {
		fillEmptyStrings(url);
	}
}

void fillEmptyStrings(metadata_data& metadata) {
	if (metadata.iso_language_code.empty())
		metadata.iso_language_code = generateString();
	if (metadata.result_type.empty())
		metadata.result_type = generateString();
}

void fillEmptyStrings(status_entities& entities) {
	if (entities.media) {
		for (auto& media: *entities.media) {
			fillEmptyStrings(media);
		}
	}
	for (auto& mention: entities.user_mentions) {
		fillEmptyStrings(mention);
	}
	for (auto& tag: entities.hashtags) {
		fillEmptyStrings(tag);
	}
	for (auto& url: entities.urls) {
		fillEmptyStrings(url);
	}
}

void fillEmptyStrings(media_data& media) {
	if (media.source_status_id_str && media.source_status_id_str->empty())
		media.source_status_id_str = generateString();
	if (media.media_url_https.empty())
		media.media_url_https = generateString();
	if (media.expanded_url.empty())
		media.expanded_url = generateString();
	if (media.display_url.empty())
		media.display_url = generateString();
	if (media.media_url.empty())
		media.media_url = generateString();
	if (media.id_str.empty())
		media.id_str = generateString();
	if (media.type.empty())
		media.type = generateString();
	if (media.url.empty())
		media.url = generateString();

	fillEmptyStrings(media.sizes);
}

void fillEmptyStrings(sizes_data& sizes) {
	fillEmptyStrings(sizes.medium);
	fillEmptyStrings(sizes.small);
	fillEmptyStrings(sizes.thumb);
	fillEmptyStrings(sizes.large);
}

void fillEmptyStrings(large_data& large) {
	if (large.resize.empty())
		large.resize = generateString();
}

void fillEmptyStrings(url_data& url) {
	if (url.expanded_url.empty())
		url.expanded_url = generateString();
	if (url.display_url.empty())
		url.display_url = generateString();
	if (url.url.empty())
		url.url = generateString();
}

void fillEmptyStrings(user_mention& mention) {
	if (mention.screen_name.empty())
		mention.screen_name = generateString();
	if (mention.id_str.empty())
		mention.id_str = generateString();
	if (mention.name.empty())
		mention.name = generateString();
}

void fillEmptyStrings(hashtag& tag) {
	if (tag.text.empty())
		tag.text = generateString();
}

template<typename T, std::size_t N> constexpr auto convertRawToArray(T (&values)[N]) {
	std::array<std::remove_const_t<T>, N> result{};
	for (std::size_t i = 0; i < N; ++i) {
		result[i] = values[i];
	}
	return result;
}

int main() {

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray01[]{ { "since_id_str", "since_id_str" }, { "next_results", "next_results" },
		{ "refresh_url", "refresh_url" }, { "max_id_str", "max_id_str" }, { "completed_in", "completed_in" }, { "query", "query" }, { "since_id", "since_id" },
		{ "count", "count" }, { "max_id", "max_id" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray02[]{ { "indices", "indices" }, { "text", "text" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray03[]{ { "resize", "resize" }, { "w", "w" }, { "h", "h" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray04[]{ { "medium", "medium" }, { "small", "small" }, { "thumb", "thumb" }, { "large", "large" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray05[]{ { "source_status_id_str", "source_status_id_str" },
		{ "source_status_id", "source_status_id" }, { "indices", "indices" }, { "media_url_https", "media_url_https" }, { "expanded_url", "expanded_url" },
		{ "display_url", "display_url" }, { "media_url", "media_url" }, { "id_str", "id_str" }, { "type", "type" }, { "sizes", "sizes" }, { "url", "url" }, { "id", "id" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray06[]{ { "indices", "indices" }, { "expanded_url", "expanded_url" },
		{ "display_url", "display_url" }, { "url", "url" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray07[]{ { "indices", "indices" }, { "screen_name", "screen_name" }, { "id_str", "id_str" },
		{ "name", "name" }, { "id", "id" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray08[]{ { "media", "media" }, { "user_mentions", "user_mentions" }, { "symbols", "symbols" },
		{ "hashtags", "hashtags" }, { "urls", "urls" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray09[]{ { "iso_language_code", "iso_language_code" }, { "result_type", "result_type" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray10[]{ { "urls", "urls" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray11[]{ { "url", "url" }, { "description", "description" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray12[]{ { "profile_background_image_url_https", "profile_background_image_url_https" },
		{ "profile_banner_url", "profile_banner_url" }, { "profile_sidebar_border_color", "profile_sidebar_border_color" },
		{ "profile_background_image_url", "profile_background_image_url" }, { "profile_sidebar_fill_color", "profile_sidebar_fill_color" }, { "time_zone", "time_zone" },
		{ "profile_background_color", "profile_background_color" }, { "profile_image_url_https", "profile_image_url_https" }, { "utc_offset", "utc_offset" }, { "url", "url" },
		{ "profile_text_color", "profile_text_color" }, { "profile_link_color", "profile_link_color" }, { "profile_image_url", "profile_image_url" },
		{ "profile_use_background_image", "profile_use_background_image" }, { "is_translation_enabled", "is_translation_enabled" },
		{ "default_profile_image", "default_profile_image" }, { "contributors_enabled", "contributors_enabled" }, { "follow_request_sent", "follow_request_sent" },
		{ "favourites_count", "favourites_count" }, { "description", "description" }, { "screen_name", "screen_name" }, { "followers_count", "followers_count" },
		{ "statuses_count", "statuses_count" }, { "created_at", "created_at" }, { "entities", "entities" }, { "friends_count", "friends_count" },
		{ "default_profile", "default_profile" }, { "listed_count", "listed_count" }, { "location", "location" }, { "protected", "user_protected" },
		{ "is_translator", "is_translator" }, { "id_str", "id_str" }, { "notifications", "notifications" }, { "string", "string" }, { "name", "name" },
		{ "geo_enabled", "geo_enabled" }, { "lang", "lang" }, { "following", "following" }, { "verified", "verified" }, { "id", "id" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray13[]{ { "in_reply_to_status_id_str", "in_reply_to_status_id_str" },
		{ "in_reply_to_user_id_str", "in_reply_to_user_id_str" }, { "in_reply_to_screen_name", "in_reply_to_screen_name" }, { "in_reply_to_status_id", "in_reply_to_status_id" },
		{ "in_reply_to_user_id", "in_reply_to_user_id" }, { "possibly_sensitive", "possibly_sensitive" }, { "contributors", "contributors" }, { "coordinates", "coordinates" },
		{ "place", "place" }, { "geo", "geo" }, { "entities", "entities" }, { "favorite_count", "favorite_count" }, { "metadata", "metadata" }, { "created_at", "created_at" },
		{ "retweet_count", "retweet_count" }, { "source", "source" }, { "id_str", "id_str" }, { "user", "user" }, { "lang", "lang" }, { "text", "text" },
		{ "truncated", "truncated" }, { "favorited", "favorited" }, { "retweeted", "retweeted" }, { "id", "id" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray14[]{ { "retweeted_status", "retweeted_status" },
		{ "in_reply_to_status_id_str", "in_reply_to_status_id_str" }, { "in_reply_to_user_id_str", "in_reply_to_user_id_str" },
		{ "in_reply_to_screen_name", "in_reply_to_screen_name" }, { "in_reply_to_status_id", "in_reply_to_status_id" }, { "in_reply_to_user_id", "in_reply_to_user_id" },
		{ "possibly_sensitive", "possibly_sensitive" }, { "contributors", "contributors" }, { "coordinates", "coordinates" }, { "place", "place" }, { "geo", "geo" },
		{ "entities", "entities" }, { "favorite_count", "favorite_count" }, { "metadata", "metadata" }, { "created_at", "created_at" }, { "retweet_count", "retweet_count" },
		{ "source", "source" }, { "id_str", "id_str" }, { "user", "user" }, { "lang", "lang" }, { "text", "text" }, { "truncated", "truncated" }, { "favorited", "favorited" },
		{ "retweeted", "retweeted" }, { "id", "id" } };

	static constexpr std::pair<std::string_view, jsonifier::string_view> newArray15[]{ { "search_metadata", "search_metadata" }, { "statuses", "statuses" } };
	
	auto jsonifierMapNew01{ jsonifier_internal::makeSet<search_metadata_data>() };
	auto glazeMapNew01{ glz::detail::make_map<search_metadata_data>() };
	std::cout << "CURRENT TYPE: " << typeid(jsonifierMapNew01).name() << std::endl; 
	std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew01.stringScalingFactorIndex << std::endl;
	std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew01.bucketSize << std::endl;

	auto jsonifierMapNew02{ jsonifier_internal::makeSet<hashtag>() };
	auto glazeMapNew02{ glz::detail::make_map<hashtag>() };
	//std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew02.stringScalingFactorIndex << std::endl;
	//std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew02.bucketSize << std::endl;

	auto jsonifierMapNew03{ jsonifier_internal::makeSet<large_data>() };
	auto glazeMapNew03{ glz::detail::make_map<large_data>() };
	std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew03.stringScalingFactorIndex << std::endl;
	std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew03.bucketSize << std::endl;

	auto jsonifierMapNew04{ jsonifier_internal::makeSet<sizes_data>() };
	auto glazeMapNew04{ glz::detail::make_map<sizes_data>() };
	std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew04.stringScalingFactorIndex << std::endl;
	std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew04.bucketSize << std::endl;

	auto jsonifierMapNew05{ jsonifier_internal::makeSet<media_data>() };
	auto glazeMapNew05{ glz::detail::make_map<media_data>() };
	std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew05.stringScalingFactorIndex << std::endl;
	std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew05.bucketSize << std::endl;

	auto jsonifierMapNew06{ jsonifier_internal::makeSet<url_data>() };
	auto glazeMapNew06{ glz::detail::make_map<url_data>() };
	std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew06.stringScalingFactorIndex << std::endl;
	std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew06.bucketSize << std::endl;

	auto jsonifierMapNew07{ jsonifier_internal::makeSet<user_mention>() };
	auto glazeMapNew07{ glz::detail::make_map<user_mention>() };
	std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew07.stringScalingFactorIndex << std::endl;
	std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew07.bucketSize << std::endl;

	auto jsonifierMapNew08{ jsonifier_internal::makeSet<status_entities>() };
	auto glazeMapNew08{ glz::detail::make_map<status_entities>() };
	std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew08.stringScalingFactorIndex << std::endl;
	std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew08.bucketSize << std::endl;

	auto jsonifierMapNew09{ jsonifier_internal::makeSet<metadata_data>() };
	auto glazeMapNew09{ glz::detail::make_map<metadata_data>() };

	auto jsonifierMapNew10{ jsonifier_internal::makeSet<description_data>() };
	auto glazeMapNew10{ glz::detail::make_map<description_data>() };
	std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew10.stringScalingFactorIndex << std::endl;
	std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew10.bucketSize << std::endl;

	auto jsonifierMapNew11{ jsonifier_internal::makeSet<user_entities>() };
	auto glazeMapNew11{ glz::detail::make_map<user_entities>() };
	std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew11.stringScalingFactorIndex << std::endl;
	std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew11.bucketSize << std::endl;

	auto jsonifierMapNew12{ jsonifier_internal::makeSet<twitter_user>() };
	auto glazeMapNew12{ glz::detail::make_map<twitter_user>() };
	std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew12.stringScalingFactorIndex << std::endl;
	std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew12.bucketSize << std::endl;

	auto jsonifierMapNew13{ jsonifier_internal::makeSet<retweeted_status_data>() };
	auto glazeMapNew13{ glz::detail::make_map<retweeted_status_data>() };
	std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew13.stringScalingFactorIndex << std::endl;
	std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew13.bucketSize << std::endl;

	auto jsonifierMapNew14{ jsonifier_internal::makeSet<status_data>() };
	auto glazeMapNew14{ glz::detail::make_map<status_data>() };
	std::cout << "CURRENT STRINGSCALINGFACTORINDEX: " << jsonifierMapNew14.stringScalingFactorIndex << std::endl;
	std::cout << "CURRENT BUCKET SIZE: " << jsonifierMapNew14.bucketSize << std::endl;

	auto jsonifierMapNew15{ jsonifier_internal::makeSet<twitter_message>() };
	auto glazeMapNew15{ glz::detail::make_map<twitter_message>() };


	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::search_metadata_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray01); ++y) {
				auto newString = *glazeMapNew01.find(newArray01[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::search_metadata_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray01); ++y) {
				auto newString = *jsonifierMapNew01.find(newArray01[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::hashtag: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray02); ++y) {
				auto newString = *glazeMapNew02.find(newArray02[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::hashtag: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray02); ++y) {
				auto newString = *jsonifierMapNew02.find(newArray02[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::large_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray03); ++y) {
				auto newString = *glazeMapNew03.find(newArray03[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::large_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray03); ++y) {
				auto newString = *jsonifierMapNew03.find(newArray03[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::sizes_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray04); ++y) {
				auto newString = *glazeMapNew04.find(newArray04[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::sizes_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray04); ++y) {
				auto newString = *jsonifierMapNew04.find(newArray04[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::media_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray05); ++y) {
				auto newString = *glazeMapNew05.find(newArray05[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::media_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray05); ++y) {
				auto newString = *jsonifierMapNew05.find(newArray05[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::url_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray06); ++y) {
				auto newString = *glazeMapNew06.find(newArray06[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::url_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray06); ++y) {
				auto newString = *jsonifierMapNew06.find(newArray06[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::user_mention: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray07); ++y) {
				auto newString = *glazeMapNew07.find(newArray07[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::user_mention: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray07); ++y) {
				auto newString = *jsonifierMapNew07.find(newArray07[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::status_entities: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray08); ++y) {
				auto newString = *glazeMapNew08.find(newArray08[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::status_entities: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray08); ++y) {
				auto newString = *jsonifierMapNew08.find(newArray08[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::metadata_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray09); ++y) {
				auto newString = *glazeMapNew09.find(newArray09[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::metadata_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray09); ++y) {
				auto newString = *jsonifierMapNew09.find(newArray09[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::description_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray10); ++y) {
				auto newString = *glazeMapNew10.find(newArray10[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::description_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray10); ++y) {
				auto newString = *jsonifierMapNew10.find(newArray10[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::user_entities: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray11); ++y) {
				auto newString = *glazeMapNew11.find(newArray11[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::user_entities: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray11); ++y) {
				auto newString = *jsonifierMapNew11.find(newArray11[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::twitter_user: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray12); ++y) {
				auto newString = *glazeMapNew12.find(newArray12[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::twitter_user: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray12); ++y) {
				auto newString = *jsonifierMapNew12.find(newArray12[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::retweeted_status_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray13); ++y) {
				auto newString = *glazeMapNew13.find(newArray13[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::retweeted_status_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray13); ++y) {
				auto newString = *jsonifierMapNew13.find(newArray13[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::status_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray14); ++y) {
				auto newString = *glazeMapNew14.find(newArray14[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::status_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray14); ++y) {
				auto newString = *jsonifierMapNew14.find(newArray14[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::twitter_message: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray15); ++y) {
				auto newString = *glazeMapNew15.find(newArray15[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::twitter_message: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(newArray15); ++y) {
				auto newString = *jsonifierMapNew15.find(newArray15[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	
	bnch_swt::benchmark_suite<"Find Test">::writeJsonData("../../test.json");
	bnch_swt::benchmark_suite<"Find Test">::printResults();
	return 0;
}
