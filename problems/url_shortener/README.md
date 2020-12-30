# URL Shortener

From Wikipedia:

> **URL shortening** is a technique on the WWW in which a URL may be made substantially shorter
> and still direct to the required page. This is achieved by using a redirect
> which links to the web page that has a long URL. For example,
> the URL "https://example.com/assets/category_B/subcategory_C/Foo/" can be shortened
> to "https://example.com/Foo", and the URL "https://en.wikipedia.org/wiki/URL_shortening"
> can be shortened to "https://w.wiki/U". Often the redirect domain name is shorter than the original one.
> A friendly URL may be desired for messaging technologies that limit the number of characters in a message
> (for example SMS), for reducing the amount of typing required if the reader
> is copying a URL from a print source, for making it easier for a person to remember,
> or for the intention of a permalink.

# Exploration

### Data Requirements

_What size of destination URL do we plan to support?_

Usual restriction in browser address bar is around 2048 symbols.
In CDNs usual limits are in range of 8K -- 32K symbols.

_Do we need to have possibility to edit stored destination URL?_

For example, if the targeted market is corporate clients and we want to give them ability
to have actual link to "today" meeting agenda by some human readable short link.
In this case it'll be useful to have editable destination URL for some
short URL like: `https://example.com/meeting_agenda`

_Do we want to provide analytics? If yes, what kind of analytics do we want to provide?_

For instance, if our clients will use short links for their marketing campaigns or referral programs
it could be convenient for them to have analytics info about clicks.
The possible analytics could be: number of clicks, browser info, computer platform, geo locations.
Analytics could be provide by adding `.info` to the short URL like `goo.gl` did.

_Will we provide expiring or permanent storage for short and destination URLs?_

We can keep pair of short and destination URLs for specified time.
Or we can keep them for some time after the last click was registered.
The alternative is to keep short and destination URLs permanently.

_Will it possible to have several short URLs which will point to the same destination URL?_

If we plan to have many posted duplicates of destination URLs and they are not editable
maybe it makes sense to have 1-to-1 relationship. In this case we can return the same short URL
for all users who will post the same destination one.

### Biz Logic Requirements

_Do we need to provide redirect options?_

It's possible to provide redirect options by language, by country, by device.

_Do we need to cleanup/replace UTM parameters in destination URLs?_

For instance, if we want to provide some service for marketing it makes sense to
offer possibility to update UTM parameters from predefined sets.

_Do we need to support human readable short URLs?_

It's possible to provide user to define what short URL will be for specified
destination URL. In this case short URL won't generated automatically.

### Operational Requirements

_What GET rate do we expect to have to extract destination URL by short one?_

_What POST rate do we expect to have to add new destination URLs to the service?_

_What reliability do we expect from this service?_

My assumption is that it should be highly reliable to get info (to not block the business of our clients).
Discussion could be around how reliable it should be about adding new short links.

### Security and Privacy Requirements

_Do we need to have TLS support for our URL shortener service?_

To improve our users privacy and safety we probably need to add TLS support.

_Which URI schemes in destination URL do we need to support?_

Probably we need to filter some URI schemes like `data:` or `javasript:` to prevent
XSS and session hijacking attacks.

_Do we need to filter destination URLs by some bad-site screening services?_

Unfortunately url shorteners often are used by spammers and scammers.
To prevent such usage it makes sense to have some filtration mechanism
if the source of destination URLs can be non-trusted.

_Do we need to have "preview" function to show the destination URL by short URL?_

To give opportunity for users to uncover the destination URL for the certain short one
we can provide "preview" function. For instance, by adding `.preview` at the end of a short URL.
It make sense if the source of destination URL can be non-trusted.

_Do we need to provide authentication and authorization?_

For instance, for corporate usage if we want to restrict access to specified resources.

_Do we want to add random part to generated short URLs?_

_Do we want to have anti-robot functionality?_

_Do we want to add throttling for identified high intensity users?_

Using backpressure, throttling, random part in generated short URLs we can
complicate life of scanners to protect private info
(e.g. some urls can contain info about location or search terms).

## Selected Use Cases

1. Free URL Shortener with Analytics and Traffic Data for Sale
1. Feature-Rich URL Shortener for Intra-Company Usage

# Use Case #1: Free URL Shortener with Analytics and Traffic Data for Sale

In this use case potential business model could consist from 2 parts:

1. Sell analytics for URL owners
1. Sell traffic data for marketing agencies

It means that we need to focus on providing high quality post-processing of collected traffic data.
Service should be convenient for people who create short URLs, but this is not our business.
If the absence of some feature won't lead to reduce amount of traffic we don't have to implement this feature.

### Data Model

Data model consist from the parts below:

1. URL data (short_url, dest_url, user_id, created_at, clicked_at)
1. Cache data (short_url, dest_url)
1. Traffic data: clicks, geo (country, region, city), language, browser, device
1. Config data in CP storage

Because of free model we probably will have a LOT of short URLs in our database (DB).
That means we should select more short version of stored destination URLs: 2048 symbols max
(usual browser address line limit).

```
CREATE TABLE IF NOT EXISTS users (
    id INT NOT NULL AUTO_INCREMENT,
    login VARCHAR(255) NOT NULL,
    pw_hash VARCHAR(255) NOT NULL

    PRIMARY KEY (id)
) ENGINE=INNODB;

CREATE TABLE IF NOT EXISTS urls (
    short_url CHAR(9) NOT NULL,
    dest_url VARCHAR(2048) NOT NULL,
    user_id INT,
    owner VARCHAR(255),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    clicked_at TIMESTAMP,

    PRIMARY KEY (short_url),
    FOREIGN KEY (user_id) REFERENCES users(id)
)  ENGINE=INNODB;
```

**TODO: cache data**
**TODO: traffic data**
**TODO: config data**

### Quantitative Analysis

Let's suppose that the common place for exchanging such short URLs is social networks and chats.
Let's assume that in Facebook + Instagram + WhatsApp we have about 3 billion of daily active users (DAU).

Below I collected assumptions for further calculations:

1. About creating short URLs
   1. 1% of DAU are active content generators
   1. Each active content generator write about 10 posts per day
   1. In 10% of these posts active content generator use 1 new short URL
1. About reading destination URLs
   1. 99% of DAU are passive readers
   1. Each passive reader has about 1000 friends+groups in average.
   1. Passive readers read only 10% of generated posts
   1. Passive readers click on 10% of links in read posts
1. About spikes
   1. Some celebrities could have up to 100 million subscribers
   1. 10% of celebrities subscribers are read their posts in the first hour
   1. Celebrities subscribers usually click on 10% of links in read posts

All calculations below will in units per second.

```
sec_per_hour = 60 * 60 = 3600
sec_per_day = 24 * sec_per_hour = 86400

POST requests:
==============
generated_posts = DAU * %_active_content_generators * posts_per_day / sec_per_day
generated_posts = 3 * 10^9 * 0.01 * 10 / 86400 = 3472 [posts / sec]

new_short_urls = generated_posts * %_posts_with_new_short_url
new_short_urls = 3472 * 0.1 = 347 [urls / sec]
new_short_urls = 347 * sec_per_day * 356  ~ 11 * 10^9 [urls / year]

GET requests (avg rate):
========================
read_posts = DAU * %_passive_readers * avg_#_of_friends * %_active_content_generators * posts_per_day / sec_per_day
read_posts = 3 * 10^9 * 0.99 * 1000 * 0.01 * 10 / 86400 = 3437500 [posts / sec]

read_dest_urls = read_posts * %_posts_with_new_short_url * %_clicks_in_read_posts
read_dest_urls = 3437500 * 0.1 * 0.1 = 34375 [urls / sec]

GET requests (spike for 1 url):
===============================
read_dest_url = #_celebrity_subscribers * %_subscribers_read_in_1st_hour / sec_per_hour
read_dest_url = 100 * 10^6 * 0.1 / 3600 = 2778 [read / sec]
```

Because we estimated the amount of used short URLs per year in 11 _ 10^9
it means that we can choose 7 symbols for short URL: 62 ^ 7 / (11 _ 10^9) ~ 320 years.
So it'll take about 320 years to exhaust all short URLs in this service.

### Component Decomposition

### Trade-off Analysis

Size of random part in short URL
TTL in cache vs number of replicas in DB
Granulation of acquired ranges in CP storage

### Implementation Details

# Use Case #2: Feature-Rich URL Shortener for Intra-Company Usage
