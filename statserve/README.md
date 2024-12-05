# StatServe server


## Protocol

- `statserve create <stat_name>`
- `statserve mean <stat_name>`
- `statserve sample <stat_name> value`
- `statserve dump <stat_name>`

Protocol returns
- `OK`
- `DNE` (does not exist)
- `ERR`
- `EXISTS` (already exists)

