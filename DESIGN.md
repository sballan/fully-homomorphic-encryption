# Design

## The Database
- It's a sequence of key value pairs
- Keys and values are stored in separate char arrays
- The key and the value char arrays each have a corresponding index array
- Max size of all arrays are 256 (limitation of the library)

## Operations
- MUST: Query by title name (exact match)
- Query by title name (partial match)
- Count all notes
- Count notes exactly matching string 
- Count notes partially matching string 
- MUST: Insert new note

Choice of operation is also encrypted for maximum security

If database is paged, you always need to iterate over all pages.

## Homomorphic Functions
MAX_ARRAY_SIZE = 256;
```
void database_operation(
  char keys_db[MAX_ARRAY_SIZE], 
  int keys_idx[MAX_ARRAY_SIZE], 
  char values_db[MAX_ARRAY_SIZE], 
  int values_idx[MAX_ARRAY_SIZE], 
  int query_type, 
  char query_key[MAX_ARRAY_SIZE], 
  char query_value[MAX_ARRAY_SIZE], 
  char result[MAX_ARRAY_SIZE]
) {

}
```


