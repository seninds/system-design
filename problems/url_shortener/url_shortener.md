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

## Exploration

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

1. Free URL Shortener to Collect Traffic Data for Sell
1. Free URL Shortener to Provide Rich Analytics for Marketing
1. Feature-Rich URL Shortener for Intra-Company Usage

The difference in implementation between the first and the second use case
are not too big. Therefore, I propose for further investigation the first and the last cases.

# Use Case #1: Free URL Shortener to Collect Traffic Data for Sell

### Data Model

### Quantitative Analysis

### Component Decomposition

### Trade-off Analysis

### Implementation Details

# Use Case #2: Feature-Rich URL Shortener for Intra-Company Usage
