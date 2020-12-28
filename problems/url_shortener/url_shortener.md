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

### Business Requirements

_Do we need to have possibility to edit stored destination URL?_

For example, if the targeted market is corporate clients and we want to give them ability
to have actual link to "today" meeting agenda by some human readable short link.
In this case it'll be useful to have editable destination URL for some
short URL like: `https://example.com/meeting_agenda`

- Analytics
- UTM presets
- Redirect Options (for instance: by language, by country, by device)
- Expiring or Permanent Storage
- Short and Destination Links Relationship
- Deep URLs support
- Human readable links

### Security Requirements

_Which URI schemes in destination URL do we need to support?_

The standard practice is to not support such URI schemes like `data:` and `javascript:`
to prevent XSS and session hijacking attacks.
Other URI schemes such as `http`, `https`, `ftp`, `ftps`, `mailto`, etc are usually supported.

_Do we need to filter destination URLs by some bad-site screening services?_

Unfortunately url shorteners often are used by spammers and scammers.
To prevent such usage it makes sense to have some filtration mechanism
if the source of destination URLs can be non-trusted.

_Do we need to have "preview" function to show the destination URL by short URL?_

To give opportunity for users to uncover the destination URL for the certain short one
we can provide "preview" subdomain.
It make sense if the source of destination URL can be non-trusted.

- SSL support
- Authentication and Authorization (corporate usage; private info for maps as example)
- Random salt in the short URL (to make harder to scan)

### Reliability Requirements

_What reliability do we expect from this service?_

My assumption is that it should be highly reliable to get info (to not block the business of our clients).
Discussion could be around how reliable it should be about adding new short links.

## Selected Use Cases

# Use Case #1: TODO name

### Data Model

### Quantitative Analysis

### Component Decomposition

### Trade-off Analysis

### Implementation Details
