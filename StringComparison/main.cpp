#include "BenchmarkSuite.hpp"
#include "glaze/glaze.hpp"
#include <concepts>// Required for concepts
#include "../StringComparison/jsonifier/Index.hpp"

struct icon_emoji_data {
	std::optional<std::string> name{};
	std::nullptr_t id{ nullptr };
};

struct permission_overwrite {
	std::string allow{};
	std::string deny{};
	int32_t test{};
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
		&value_type::video_quality_mode, &value_type::application_id, &value_type::permissions, &value_type::message_count, &value_type::parent_id, &value_type::member_count,
		&value_type::owner_id, &value_type::guild_id, &value_type::user_limit, &value_type::position, &value_type::name, &value_type::icon, &value_type::version,
		&value_type::bitrate, &value_type::id, &value_type::flags, &value_type::type, &value_type::managed, &value_type::nsfw>();
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
		&value_type::unavailable, "id", &value_type::id, "flags", &value_type::flags, "large", &value_type::large, "owner", &value_type::owner, "nsfw", &value_type::nsfw, "lazy",
		&value_type::lazy);
};

template<> struct jsonifier::core<discord_message> {
	using value_type				 = discord_message;
	static constexpr auto parseValue = createValue<&value_type::t, &value_type::d, &value_type::op, &value_type::s>();
};

struct test_struct {
	std::vector<std::string> testStrings{};
	std::vector<uint64_t> testUints{};
	std::vector<double> testDoubles{};
	std::vector<int64_t> testInts{};
	std::vector<bool> testBools{};
};

template<typename value_type> struct test {
	std::vector<value_type> a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z;
};

#include <random>

template<typename value_type> struct test_generator {
	std::vector<value_type> a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z;

	inline static std::random_device randomEngine{};
	inline static std::mt19937_64 gen{ randomEngine() };

	static constexpr std::string_view charset{ "!#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~\"\\\r\b\f\t\n" };

	template<typename value_type_new> static value_type_new randomizeNumberNormal(value_type_new mean, value_type_new stdDeviation) {
		std::normal_distribution<> normalDistributionTwo{ static_cast<double>(mean), static_cast<double>(stdDeviation) };
		auto theResult = normalDistributionTwo(randomEngine);
		if (theResult < 0) {
			theResult = -theResult;
		}
		return theResult;
	}

	template<typename value_type_new> static value_type_new randomizeNumberUniform(value_type_new range) {
		std::uniform_int_distribution<uint64_t> dis(0, range);
		return dis(randomEngine);
	}

	static void insertUnicodeInJSON(std::string& jsonString) {
	}

	static std::string generateString(uint64_t length) {
		static constexpr int32_t charsetSize = charset.size();
		auto unicodeCount					 = randomizeNumberUniform(length / 8);
		std::string result{};
		for (int32_t x = 0; x < length; ++x) {
			if (x == static_cast<int32_t>(length)) {
				insertUnicodeInJSON(result);
			}
			result += charset[randomizeNumberUniform(charsetSize - 1)];
		}
		return result;
	}

	double generateDouble() {
		auto newValue = randomizeNumberNormal(double{}, std::numeric_limits<double>::max() / 50000000);
		return generateBool() ? newValue : -newValue;
	};

	bool generateBool() {
		return static_cast<bool>(randomizeNumberNormal(50.0f, 50.0f) >= 50.0f);
	};

	uint64_t generateUint() {
		return randomizeNumberNormal(std::numeric_limits<uint64_t>::max() / 2, std::numeric_limits<uint64_t>::max() / 2);
	};

	int64_t generateInt() {
		auto newValue = randomizeNumberNormal(int64_t{}, std::numeric_limits<int64_t>::max());
		return generateBool() ? newValue : -newValue;
	};

	test_generator() {
		auto fill = [&](auto& v) {
			auto arraySize01 = randomizeNumberNormal(35ull, 10ull);
			auto arraySize02 = randomizeNumberNormal(15ull, 10ull);
			auto arraySize03 = randomizeNumberNormal(5ull, 1ull);
			v.resize(arraySize01);
			for (uint64_t x = 0; x < arraySize01; ++x) {
				auto arraySize01New = randomizeNumberNormal(arraySize02, arraySize03);
				for (uint64_t y = 0; y < arraySize01New; ++y) {
					auto newString = generateString(23);
					v[x].testStrings.emplace_back(newString);
				}
				arraySize01New = randomizeNumberNormal(arraySize02, arraySize03);
				for (uint64_t y = 0; y < arraySize01New; ++y) {
					v[x].testUints.emplace_back(generateUint());
				}
				arraySize01New = randomizeNumberNormal(arraySize02, arraySize03);
				for (uint64_t y = 0; y < arraySize01New; ++y) {
					v[x].testInts.emplace_back(generateInt());
				}
				arraySize01New = randomizeNumberNormal(arraySize02, arraySize03);
				for (uint64_t y = 0; y < arraySize01New; ++y) {
					auto newBool = generateBool();
					v[x].testBools.emplace_back(newBool);
				}
				arraySize01New = randomizeNumberNormal(arraySize02, arraySize03);
				for (uint64_t y = 0; y < arraySize01New; ++y) {
					v[x].testDoubles.emplace_back(generateDouble());
				}
			}
		};

		fill(a);
		fill(b);
		fill(c);
		fill(d);
		fill(e);
		fill(f);
		fill(g);
		fill(h);
		fill(i);
		fill(j);
		fill(k);
		fill(l);
		fill(m);
		fill(n);
		fill(o);
		fill(p);
		fill(q);
		fill(r);
		fill(s);
		fill(t);
		fill(u);
		fill(v);
		fill(w);
		fill(x);
		fill(y);
		fill(z);
	}
};


template<typename value_type> struct abc_test {
	std::vector<value_type> z, y, x, w, v, u, t, s, r, q, p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a;
};

template<> struct jsonifier::core<test_struct> {
	using value_type				 = test_struct;
	static constexpr auto parseValue = createValue<&value_type::testStrings, &value_type::testUints, &value_type::testDoubles, &value_type::testInts, &value_type::testBools>();
};

template<> struct jsonifier::core<test<test_struct>> {
	using value_type				 = test<test_struct>;
	static constexpr auto parseValue = createValue<&value_type::a, &value_type::b, &value_type::c, &value_type::d, &value_type::e, &value_type::f, &value_type::g, &value_type::h,
		&value_type::i, &value_type::j, &value_type::k, &value_type::l, &value_type::m, &value_type::n, &value_type::o, &value_type::p, &value_type::q, &value_type::r,
		&value_type::s, &value_type::t, &value_type::u, &value_type::v, &value_type::w, &value_type::x, &value_type::y, &value_type::z>();
};

template<> struct jsonifier::core<test_generator<test_struct>> {
	using value_type				 = test_generator<test_struct>;
	static constexpr auto parseValue = createValue<&value_type::a, &value_type::b, &value_type::c, &value_type::d, &value_type::e, &value_type::f, &value_type::g, &value_type::h,
		&value_type::i, &value_type::j, &value_type::k, &value_type::l, &value_type::m, &value_type::n, &value_type::o, &value_type::p, &value_type::q, &value_type::r,
		&value_type::s, &value_type::t, &value_type::u, &value_type::v, &value_type::w, &value_type::x, &value_type::y, &value_type::z>();
};

template<> struct jsonifier::core<abc_test<test_struct>> {
	using value_type				 = abc_test<test_struct>;
	static constexpr auto parseValue = createValue<&value_type::z, &value_type::y, &value_type::x, &value_type::w, &value_type::v, &value_type::u, &value_type::t, &value_type::s,
		&value_type::r, &value_type::q, &value_type::p, &value_type::o, &value_type::n, &value_type::m, &value_type::l, &value_type::k, &value_type::j, &value_type::i,
		&value_type::h, &value_type::g, &value_type::f, &value_type::e, &value_type::d, &value_type::c, &value_type::b, &value_type::a>();
};




template<uint64_t N> using fit_unsigned_t = std::conditional_t<N <= std::numeric_limits<uint8_t>::max(), uint8_t,
	std::conditional_t<N <= std::numeric_limits<uint16_t>::max(), uint16_t, std::conditional_t<N <= std::numeric_limits<uint32_t>::max(), uint32_t, uint64_t>>>;

struct rt_group {
	using index_type = std::array<uint8_t, 16>;
	uint8_t index{ 0 };

	JSONIFIER_INLINE void extractIndices(uint32_t value) {
		index = simd_internal::tzcnt(value);
	}

	JSONIFIER_INLINE uint8_t getIndex() const {
		return index;
	}

	JSONIFIER_INLINE void match(const uint8_t* indices, uint8_t hash) {
		extractIndices(simd_internal::opCmpEq(simd_internal::gatherValue<simd_int_128>(hash), simd_internal::gatherValues<simd_int_128>(indices)));
	}
};

struct ct_group {
	using index_type = std::array<uint8_t, 16>;
	uint8_t index{ 0 };

	constexpr ct_group(const uint8_t* indices, uint8_t hash) {
		match(indices, hash);
	}

	constexpr void extractIndices(uint32_t value) {
		index = tzcnt(value);
	}

	constexpr int32_t tzcnt(uint32_t value) const {
		int32_t count = 0;
		while ((value & 1) == 0 && value != 0) {
			value >>= 1;
			++count;
		}
		return count;
	}

	constexpr uint8_t getIndex() const {
		return index;
	}

	constexpr void match(const uint8_t* indices, uint8_t hash) {
		uint32_t mask = 0;
		for (int32_t i = 0; i < 16; ++i) {
			if (indices[i] == hash) {
				mask |= (1 << i);
			}
		}
		extractIndices(mask);
	}
};

inline thread_local rt_group rtGrp{};

template<uint64_t N> using fit_unsigned_t = std::conditional_t<N <= std::numeric_limits<uint8_t>::max(), uint8_t,
	std::conditional_t<N <= std::numeric_limits<uint16_t>::max(), uint16_t, std::conditional_t<N <= std::numeric_limits<uint32_t>::max(), uint32_t, uint64_t>>>;

struct group {
	using index_type = std::array<uint8_t, 16>;

	constexpr int32_t tzcnt(uint32_t value) const {
		int32_t count = 0;
		while ((value & 1) == 0 && value != 0) {
			value >>= 1;
			++count;
		}
		return count;
	}

	constexpr uint8_t match(const uint8_t* indices, uint8_t hash) const {
		if (std::is_constant_evaluated()) {
			uint32_t mask = 0;
			for (int32_t i = 0; i < 16; ++i) {
				if (indices[i] == hash) {
					mask |= (1 << i);
				}
			}
			return tzcnt(mask);
		} else {
			return simd_internal::tzcnt(simd_internal::opCmpEq(simd_internal::gatherValue<simd_int_128>(hash), simd_internal::gatherValues<simd_int_128>(indices)));
		}
	}
};

template<typename key_type, typename value_type, size_t N> struct simd_map : public jsonifier_internal::fnv1a_hash, public group {
	static constexpr uint64_t storageSize = ((N + 15) / 16) * 16;
	static constexpr auto maxBucketSize	  = 16;
	static constexpr uint64_t numGroups	  = (storageSize / 16);
	uint64_t seed{};
	using hasher = jsonifier_internal::fnv1a_hash;
	std::array<glz::pair<key_type, value_type>, storageSize> items{};
	std::array<uint8_t, storageSize> indices{};
	std::array<uint64_t, storageSize> hashes{};

	constexpr auto begin() const noexcept {
		return items.begin();
	}

	constexpr auto end() const noexcept {
		return items.end();
	}

	constexpr size_t size() const noexcept {
		return N;
	}

	constexpr auto find(const key_type& key) const noexcept {
		auto hash			 = hasher::operator()(key.data(), key.size(), seed);
		size_t groupPos		 = hash % numGroups;
		size_t startGroupPos = groupPos;

		auto resultIndex = group::match(indices.data() + groupPos * 16, H2(hash));
		if (hashes[groupPos * 16 + resultIndex] == hash) {
			return items.begin() + groupPos * 16 + resultIndex;
		}
		return items.end();
	}

	constexpr simd_map(const std::array<glz::pair<key_type, value_type>, N>& pairs) : items{}, indices{} {
		if constexpr (N == 0) {
			return;
		}

		std::array<size_t, numGroups> bucketSizes{};
		seed = 1;
		bool failed{};

		do {
			std::fill(indices.begin(), indices.end(), 0);
			std::fill(items.begin(), items.end(), glz::pair<key_type, value_type>{});
			std::fill(bucketSizes.begin(), bucketSizes.end(), 0);

			failed = false;
			for (size_t i = 0; i < N; ++i) {
				const auto hash		  = hasher::operator()(pairs[i].first.data(), pairs[i].first.size(), seed);
				const auto groupPos	  = hash % numGroups;
				const auto bucketSize = bucketSizes[groupPos]++;
				const auto ctrlByte	  = H2(hash);

				if (bucketSize >= maxBucketSize || doesItContainIt(indices.data() + groupPos * 16, ctrlByte)) {
					failed				  = true;
					bucketSizes[groupPos] = 0;
					++seed;
					break;
				}

				indices[groupPos * 16 + bucketSize] = ctrlByte;
				items[groupPos * 16 + bucketSize]		 = pairs[i];
				hashes[groupPos * 16 + bucketSize]		 = hash;
			}
		} while (failed);
	}

	constexpr uint8_t H2(size_t hash) const noexcept {
		return static_cast<uint8_t>(hash & 0xFF);
	}

	constexpr bool doesItContainIt(const uint8_t* ctrBytes, uint8_t byteToCheckFor) const {
		for (uint64_t x = 0; x < 16; ++x) {
			if (ctrBytes[x] == byteToCheckFor) {
				return true;
			}
		}
		return false;
	}
};

constexpr uint8_t H2(size_t hash) noexcept {
	return static_cast<uint8_t>(hash & 0xFF);
}

#include <random>

inline void generateEqualHashTest(jsonifier::string_view stringToTest) {
	jsonifier_internal::fnv1a_hash hash{};
	auto newHash = H2(hash.operator()(stringToTest.data(), stringToTest.size(), 0));
	uint64_t newerHash{};
	uint64_t index{};
	jsonifier::string newerString{};
	while (newHash != newerHash) {
		newerString = test_generator<int32_t>::generateString(stringToTest.size());
		newerHash	= H2(hash.operator()(newerString.data(), newerString.size(), 0));
		std::cout << "CURRENT HASH: " << +newerHash << std::endl;
		++index;
	}
	std::cout << "IT TOOK: " << index << " ITERATIONS: " << newerString << std::endl;

}

template<typename T, std::size_t N> constexpr auto convertRawToArray(T (&values)[N]) {
	std::array<std::remove_const_t<T>, N> result{};
	for (std::size_t i = 0; i < N; ++i) {
		result[i] = values[i];
	}
	return result;
}

static inline uint64_t load64(const uint8_t* ptr) {
	uint64_t val;
	memcpy(&val, ptr, sizeof(uint64_t));
	return val;
}

// Generate the mask using bitwise operations
static inline uint64_t generate_mask(uint64_t control, uint8_t hash) {
	uint64_t hash_mask = 0x0101010101010101ULL * hash;// Broadcast hash to all bytes
	uint64_t cmp	   = control ^ hash_mask;// XOR to find bytes that match

	// Subtract 1 from each byte in the comparison result
	uint64_t sub = cmp - 0x0101010101010101ULL;

	// Mask to isolate the high bit of each byte
	uint64_t high_bits = ~cmp & 0x8080808080808080ULL;

	// If a byte is zero, the high bit will be set in the result after subtraction
	uint64_t result = (sub & high_bits) >> 7;

	// Extract the mask
	uint64_t mask = (result & 0x0101010101010101ULL) * 0x8040201008040201ULL >> 56;
	return mask;
}

template<uint64_t size> inline uint8_t match(const uint8_t* indices, uint8_t hash);
template<> inline uint8_t match<8>(const uint8_t* indices, uint8_t hash) {
	uint64_t packedControlBytes;
	std::memcpy(&packedControlBytes, indices, sizeof(uint64_t));

	uint64_t packedHash;
	std::memset(&packedControlBytes, hash, sizeof(uint64_t));

	uint64_t comparisonResult	   = ~(packedControlBytes ^ packedHash);
	static constexpr uint64_t mask = jsonifier_internal::repeatByte<0x01, uint64_t>();

	comparisonResult &= (comparisonResult >> 7) & mask;

	return simd_internal::tzcnt(comparisonResult);
}

template<> inline uint8_t match<4>(const uint8_t* indices, uint8_t hash) {
	uint32_t packedControlBytes;
	std::memcpy(&packedControlBytes, indices, sizeof(uint32_t));

	uint32_t packedHash;
	std::memset(&packedControlBytes, hash, sizeof(uint32_t));

	uint32_t comparisonResult	   = ~(packedControlBytes ^ packedHash);
	static constexpr uint32_t mask = jsonifier_internal::repeatByte<0x01, uint32_t>();

	comparisonResult &= (comparisonResult >> 7) & mask;

	return simd_internal::tzcnt(comparisonResult);
}

template<> inline uint8_t match<2>(const uint8_t* indices, uint8_t hash) {
	uint16_t packedControlBytes;
	std::memcpy(&packedControlBytes, indices, sizeof(uint16_t));

	uint16_t packedHash;
	std::memset(&packedControlBytes, hash, sizeof(uint16_t));

	uint16_t comparisonResult	   = ~(packedControlBytes ^ packedHash);
	static constexpr uint16_t mask = jsonifier_internal::repeatByte<0x01, uint16_t>();

	comparisonResult &= (comparisonResult >> 7) & mask;

	return simd_internal::tzcnt(comparisonResult);
}

template<uint64_t length> struct convert_length_to_int_type {
	static_assert(length <= 8, "Sorry, but that string is too long!");
	using type = std::conditional_t<length == 1, uint8_t,
		std::conditional_t<length <= 2, uint16_t, std::conditional_t<length <= 4, uint32_t, std::conditional_t<length <= 8, uint64_t, void>>>>;
};

template<uint64_t length> using convert_length_to_int_type_t = typename convert_length_to_int_type<length>::type;

template<jsonifier_internal::string_literal string> constexpr convert_length_to_int_type_t<string.size()> getStringAsInt() {
	const char* stringNew = string.data();
	convert_length_to_int_type_t<string.size()> returnValue{};
	for (uint64_t x = 0; x < string.size(); ++x) {
		returnValue |= static_cast<convert_length_to_int_type_t<string.size()>>(stringNew[x]) << x * 8;
	}
	return returnValue;
}

template<jsonifier_internal::string_literal string> JSONIFIER_INLINE bool compareStringAsInt(const char* iter) {
	static constexpr auto newString{ getStringAsInt<string>() };
	convert_length_to_int_type_t<string.size()> newString02{};
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

template<jsonifier_internal::simd_structural_iterator_t iterator_type> JSONIFIER_INLINE bool parseNull(iterator_type&& iter) {
	if (compareStringAsInt<"null">(iter)) {
		++iter;
		return true;
	} else {
		return false;
	}
}

int main() {
	std::string testString{ "TESTING" };
	std::cout << "STRING LENGTH: " << testString.end() - testString.begin() << std::endl;

	static constexpr std::array<glz::pair<std::string_view, jsonifier::string_view>, 4> arrayNewer00{ { { "id", "id" }, { "name", "name" } } };

	static constexpr std::array<glz::pair<std::string_view, jsonifier::string_view>, 4> arrayNewer01{ { { "deny", "deny" }, { "allow", "allow" }, { "id", "id" },
		{ "type", "type" } } };

	static constexpr std::array<glz::pair<std::string_view, jsonifier::string_view>, 34> arrayNewer02{ { { "permission_overwrites", "permission_overwrites" },
		{ "last_message_id", "last_message_id" }, { "default_thread_rate_limit_per_user", "default_thread_rate_limit_per_user" }, { "applied_tags", "applied_tags" },
		{ "recipients", "recipients" }, { "default_auto_archive_duration", "default_auto_archive_duration" }, { "rtc_region", "rtc_region" }, { "status", "status" },
		{ "last_pin_timestamp", "last_pin_timestamp" }, { "topic", "topic" }, { "rate_limit_per_user", "rate_limit_per_user" }, { "icon_emoji", "icon_emoji" },
		{ "total_message_sent", "total_message_sent" }, { "video_quality_mode", "video_quality_mode" }, { "application_id", "application_id" }, { "permissions", "permissions" },
		{ "message_count", "message_count" }, { "parent_id", "parent_id" }, { "member_count", "member_count" }, { "owner_id", "owner_id" }, { "guild_id", "guild_id" },
		{ "user_limit", "user_limit" }, { "position", "position" }, { "name", "name" }, { "icon", "icon" }, { "version", "version" }, { "bitrate", "bitrate" }, { "id", "id" },
		{ "flags", "flags" }, { "type", "type" }, { "managed", "managed" }, { "nsfw", "nsfw" } } };

	static constexpr std::array<glz::pair<std::string_view, jsonifier::string_view>, 18> arrayNewer03{ { { "avatar_decoration_data", "avatar_decoration_data" },
		{ "display_name", "display_name" }, { "global_name", "global_name" }, { "avatar", "avatar" }, { "banner", "banner" }, { "locale", "locale" },
		{ "discriminator", "discriminator" }, { "user_name", "user_name" }, { "accent_color", "accent_color" }, { "premium_type", "premium_type" },
		{ "public_flags", "public_flags" }, { "email", "email" }, { "mfa_enabled", "mfa_enabled" }, { "id", "id" }, { "flags", "flags" }, { "verified", "verified" },
		{ "system", "system" }, { "bot", "bot" } } };

	static constexpr std::array<glz::pair<std::string_view, jsonifier::string_view>, 14> arrayNewer04{ { { "communication_disabled_until", "communication_disabled_until" },
		{ "premium_since", "premium_since" }, { "nick", "nick" }, { "avatar", "avatar" }, { "roles", "roles" }, { "permissions", "permissions" }, { "joined_at", "joined_at" },
		{ "guild_id", "guild_id" }, { "user", "user" }, { "flags", "flags" }, { "pending", "pending" }, { "deaf", "deaf" }, { "mute", "mute" } } };

	static constexpr std::array<glz::pair<std::string_view, jsonifier::string_view>, 3> arrayNewer05{ { { "premium_subscriber", "premium_subscriber" }, { "bot_id", "bot_id" } } };

	static constexpr std::array<glz::pair<std::string_view, jsonifier::string_view>, 13> arrayNewer06{ { { "unicode_emoji", "unicode_emoji" }, { "icon", "icon" },
		{ "permissions", "permissions" }, { "position", "position" }, { "name", "name" }, { "mentionable", "mentionable" }, { "version", "version" }, { "id", "id" },
		{ "tags", "tags" }, { "color", "color" }, { "flags", "flags" }, { "managed", "managed" }, { "hoist", "hoist" } } };

	static constexpr std::array<glz::pair<std::string_view, jsonifier::string_view>, 59> arrayNewer07{ { { "latest_on_boarding_question_id", "latest_on_boarding_question_id" },
		{ "guild_scheduled_events", "guild_scheduled_events" }, { "safety_alerts_channel_id", "safety_alerts_channel_id" }, { "inventory_settings", "inventory_settings" },
		{ "voice_states", "voice_states" }, { "discovery_splash", "discovery_splash" }, { "vanity_url_code", "vanity_url_code" }, { "application_id", "application_id" },
		{ "afk_channel_id", "afk_channel_id" }, { "default_message_notifications", "default_message_notifications" },
		{ "max_stage_video_channel_users", "max_stage_video_channel_users" }, { "public_updates_channel_id", "public_updates_channel_id" }, { "description", "description" },
		{ "threads", "threads" }, { "channels", "channels" }, { "premium_subscription_count", "premium_subscription_count" },
		{ "approximate_presence_count", "approximate_presence_count" }, { "features", "features" }, { "stickers", "stickers" },
		{ "premium_progress_bar_enabled", "premium_progress_bar_enabled" }, { "members", "members" }, { "hub_type", "hub_type" },
		{ "approximate_member_count", "approximate_member_count" }, { "explicit_content_filter", "explicit_content_filter" },
		{ "max_video_channel_users", "max_video_channel_users" }, { "splash", "splash" }, { "banner", "banner" }, { "system_channel_id", "system_channel_id" },
		{ "widget_channel_id", "widget_channel_id" }, { "preferred_locale", "preferred_locale" }, { "system_channel_flags", "system_channel_flags" },
		{ "rules_channel_id", "rules_channel_id" }, { "roles", "roles" }, { "verification_level", "verification_level" }, { "permissions", "permissions" },
		{ "max_presences", "max_presences" }, { "discovery", "discovery" }, { "joined_at", "joined_at" }, { "member_count", "member_count" }, { "premium_tier", "premium_tier" },
		{ "owner_id", "owner_id" }, { "max_members", "max_members" }, { "afk_timeout", "afk_timeout" }, { "widget_enabled", "widget_enabled" }, { "region", "region" },
		{ "nsfw_level", "nsfw_level" }, { "mfa_level", "mfa_level" }, { "name", "name" }, { "icon", "icon" }, { "unavailable", "unavailable" }, { "id", "id" },
		{ "flags", "flags" }, { "large", "large" }, { "owner", "owner" }, { "nsfw", "nsfw" }, { "lazy", "lazy" } } };

	static constexpr auto jsonifierMapNew00{ jsonifier_internal::makeMap<icon_emoji_data>() };
	static constexpr auto glazeMapNew00{ glz::detail::make_map<icon_emoji_data>() };

	static constexpr auto jsonifierMapNew01{ jsonifier_internal::makeMap<permission_overwrite>() };
	static constexpr auto glazeMapNew01{ glz::detail::make_map<permission_overwrite>() };
	std::cout << "CURRENT TYPE: " << typeid(glazeMapNew01).name() << std::endl;

	static constexpr auto jsonifierMapNew02{ jsonifier_internal::makeMap<channel_data>() };
	static constexpr auto glazeMapNew02{ glz::detail::make_map<channel_data>() };
	//static constexpr glz::detail::normal_map<std::string_view, jsonifier::string_view, std::size(arrayNewer02)> glazeMapNew02{ arrayNewer02 };

	static constexpr auto jsonifierMapNew03{ jsonifier_internal::makeMap<user_data>() };
	static constexpr auto glazeMapNew03{ glz::detail::make_map<user_data>() };

	static constexpr auto jsonifierMapNew04{ jsonifier_internal::makeMap<member_data>() };
	static constexpr auto glazeMapNew04{ glz::detail::make_map<member_data>() };
	std::cout << "CURRENT TYPE: " << typeid(glazeMapNew04).name() << std::endl;

	static constexpr auto jsonifierMapNew05{ jsonifier_internal::makeMap<tags_data>() };
	static constexpr auto glazeMapNew05{ glz::detail::make_map<tags_data>() };

	static constexpr auto jsonifierMapNew06{ jsonifier_internal::makeMap<role_data>() };
	static constexpr auto glazeMapNew06{ glz::detail::make_map<role_data>() };

	static constexpr auto jsonifierMapNew07{ jsonifier_internal::makeMap<guild_data>() };
	static constexpr auto glazeMapNew07{ glz::detail::make_map<guild_data>() };

	for (const auto& elem: jsonifierMapNew01) {
		std::cout << elem.first << '\n';
	}

	for (const auto& elem: jsonifierMapNew03) {
		std::cout << elem.first << '\n';
	}
	//auto newFind = mapNew.find("b");
	//std::cout << "CURRENT VALUE: " << newFind.operator*().second << std::endl;
	
	uint64_t newArray[] = {
		0b000000001100000010001001010000111111111111000000000011111111100000011111,
		0b000000001100000010001001010000111111111111000000000011111111100000011111,
		0b000000001100000010001001010000111111111111000000000011111111100000011111,
		0b000000001100000010001001010000111111111111000000000011111111100000011111,
	};
	bnch_swt::file_loader raw01{ "../../../../DisordData-Prettified.json" };
	auto newerString01{ raw01.operator std::string&() };
	jsonifier::jsonifier_core parser{};

	test_generator<test_struct> testData{};
	discord_message message{};
	parser.parseJson<jsonifier::parse_options{ .minified = false }>(testData, raw01.operator std::string&());
	parser.serializeJson(testData, raw01.operator std::string&());
	for (auto& value: parser.getErrors()) {
		std::cout << "ERROR: " << value.reportError() << std::endl;
	}
	
	for (uint64_t x = 0; x < raw01.operator std::string&().size(); ++x) {
		//if (newerString01[x] == '\n') {
		//newerString01[x] = ' ';
		//}
	}
	bnch_swt::file_loader minifiedReal{ "../../../../twitterdata-minified-real.json" };
	jsonifier::string newString02{};
	jsonifier::jsonifier_core parser02{};
	//std::cout << "CURRENT TYPE: " << typeid(int32_t&).name() << std::endl;
	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::icon_emoji_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer00); ++y) {
				auto newString = glazeMapNew00.find(arrayNewer00[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::icon_emoji_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer00); ++y) {
				auto newString = jsonifierMapNew00.find(arrayNewer00[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::permission_overwrite: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer01); ++y) {
				auto newString = glazeMapNew01.find(arrayNewer01[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::permission_overwrite: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer01); ++y) {
				auto newString = jsonifierMapNew01.find(arrayNewer01[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::channel_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer02); ++y) {
				auto newString = glazeMapNew02.find(arrayNewer02[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::channel_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer02); ++y) {
				auto newString = jsonifierMapNew02.find(arrayNewer02[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::user_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer03); ++y) {
				auto newString = glazeMapNew03.find(arrayNewer03[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::user_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer03); ++y) {
				auto newString = jsonifierMapNew03.find(arrayNewer03[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::member_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer04); ++y) {
				auto newString = glazeMapNew04.find(arrayNewer04[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::member_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer04); ++y) {
				auto newString = jsonifierMapNew04.find(arrayNewer04[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::tags_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer05); ++y) {
				auto newString = glazeMapNew05.find(arrayNewer05[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::tags_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer05); ++y) {
				auto newString = jsonifierMapNew05.find(arrayNewer05[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::role_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer06); ++y) {
				auto newString = glazeMapNew06.find(arrayNewer06[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::role_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer06); ++y) {
				auto newString = jsonifierMapNew06.find(arrayNewer06[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"glz::detail::guild_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer07); ++y) {
				auto newString = glazeMapNew07.find(arrayNewer07[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});

	bnch_swt::benchmark_suite<"Find Test">::benchmark<"jsonifier_internal::guild_data: ", "steelblue", 1000>([&] {
		for (uint64_t x = 0; x < 1024; ++x) {
			for (uint64_t y = 0; y < std::size(arrayNewer07); ++y) {
				auto newString = jsonifierMapNew07.find(arrayNewer07[y].first);
				bnch_swt::doNotOptimizeAway(newString);
			}
		}
		return;
	});
	
	minifiedReal.saveFile(newString02.operator std::string());
	parser02.validateJson(newString02);
	//serializedSecond.saveFile(parser02.serializeJson(dataNew02).operator std::string());
	bnch_swt::benchmark_suite<"Find Test">::writeJsonData("../../test.json");
	/*
	for (uint64_t x = 0; x < newString03.size(); ++x) {
		if (newString03[x] != newString04[x]) {
			std::cout << "IT'S DIFFERENT AT INDEX: " << x << ", WITH VALUES 01: " << jsonifier::string_view{ newString03.data() + x, 32 }
					  << ", VALUES 02: " << jsonifier::string_view{ newString04.data() + x, 32 } << std::endl;
			break;
		}
	}*/
	return 0;
}
