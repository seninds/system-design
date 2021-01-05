# URL Shortener

[From Wikipedia](https://en.wikipedia.org/wiki/URL_shortening):

> **URL shortening** is a technique on the WWW in which a URL may be made substantially shorter
> and still direct to the required page. This is achieved by using a redirect
> which links to the web page that has a long URL.
> Often the redirect domain name is shorter than the original one.
>
> A friendly URL may be desired for messaging technologies that limit the number of characters in a message
> (for example SMS), for reducing the amount of typing required if the reader
> is copying a URL from a print source, for making it easier for a person to remember,
> or for the intention of a permalink.

# Exploration

## Data Requirements

- What size of destination URL do we plan to support?
- Do we need to have possibility to edit stored destination URLs?
- Do we want to provide analytics based on traffic data? What kind of analytics?
- Do we provide expiring or permanent storage for shorten URLs?
- Which relationships are between short and destination URLs: 1:1 or N:1?

## Biz Logic Requirements

- Do we need to provide redirect options based on language, country or platform info?
- Do we need to support human readable short URLs which are setup manually?

## Operational Requirements

- What GET rate do we expect to have to extract destination URL by short one?
- What POST rate do we expect to have to add new destination URLs to the service?
- What reliability do we expect from this service on GET and POST?
- Do we need to add back-pressure mechanism to improve reliability?

## Security and Privacy Requirements

- Do we need to have TLS support for our URL shortener service?
- Which URI schemes in destination URL do we need to support?
- Do we need to filter destination URLs by some bad-site screening services?
- Do we need to have "preview" function to show the destination URL by short URL?
- Do we need to provide user authentication and authorization for some URLs?
- Do we need to add random part to generated short URLs to prevent easy scanning?
- Do we need to have anti-robot functionality?

## Proposed Use Cases

It looks like we have 2 main use cases:

1. URL Shortener with Traffic Data Analysis:
   - free to use URL shortener
   - collect traffic data
   - sell analytics to customers
1. Feature-Rich URL Shortener for Intra-Company Usage
   - paid version of URL shortener
   - rich feature set
   - resource management (e.g. authentication and authorization)

# Use Case #1: URL Shortener with Traffic Data Analysis

In this use case potential business model could be the following.
Provide free service to create short URL for certain destination URL, but

1. Sell analytics for URL owners
1. Sell traffic data for marketing agencies

It means that we need to focus on providing high quality post-processing of collected traffic data.
Service should be convenient for people who create short URLs, but this is not the main part of our business.
If an absence of some feature won't lead to reduce amount of traffic we don't have to implement this feature.

## Data Model

Data model consists from the parts below:

1. URL data and User data
1. Cache data
1. Traffic data
1. Config data in CP storage

### URL data and User data

Because of free model we probably will have a LOT of pairs short/dest URLs in our database.
That means we should select shorter version of stored destination URLs: 2048 symbols max
(usual browser address line limit).

For short URLs I plan to use 62-based alphabet:

```c++
char alphabet[62] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
```

For short URLs we could use 7 ordered symbols (specified by counter) and
2 random ones (to complicate life for scanners).
Because we plan to use 62-based alphabet it means that the maximum number
of short URLs to create is `62^7 ~ 3.5E12`. With RPS about 10K to create a new short URL
it will take more than 10 years to exhaust this range. Looks pretty enough.

```sql
# Record size: 4 + 256 + 64 + 64 = 388 bytes
CREATE TABLE IF NOT EXISTS users (
    id INT NOT NULL AUTO_INCREMENT,
    login VARCHAR(255) NOT NULL,
    pw_hash CHAR(64) NOT NULL,
    salt CHAR(64) NOT NULL,

    PRIMARY KEY (id)
) ENGINE=INNODB;

# Record size: 9 + 2050 + 4 + 4 + 4 = 2071 bytes
CREATE TABLE IF NOT EXISTS urls (
    short_url CHAR(9) NOT NULL,
    dest_url VARCHAR(2048) NOT NULL,
    user_id INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    clicked_at TIMESTAMP,

    PRIMARY KEY (short_url),
    FOREIGN KEY (user_id) REFERENCES users(id)
)  ENGINE=INNODB;
```

_TODO: think about table for session tokens_

### Cache Data

In cache I plan to store pairs of short and corresponding destination URLs.
So the max possible size of 1 record in the cache could be: `2048 + 9 = 2057 bytes`.

_TODO: think about cache for session tokens_

**TODO: traffic data**
**TODO: config data**

## Quantitative Analysis

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

Because we estimated the amount of used short URLs per year in 11E9
it means that we can choose 7 symbols for short URL: `62 ^ 7 / 11E9 ~ 320 years`.
So it'll take about 320 years to exhaust all short URLs in this service.

## Component Decomposition

## Trade-off Analysis

Size of random part in short URL
TTL in cache vs number of replicas in DB
Granulation of acquired ranges in CP storage

## Implementation Details

# Use Case #2: Feature-Rich URL Shortener for Intra-Company Usage
