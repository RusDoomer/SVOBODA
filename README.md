# SVOBODA
Structured Validation Of Board Optimization Data API

Based almost entire on an early version of GULAG

depends on libmicrohttpd libjson-c

todo:

deprecate config.conf/args -> just have each setting be an API request, either at startup or within each request

deprecate weights -> again just a part of each API request

deprecate layout storage -> the API doesn't need to remember these whoever calls the API should have the foresight to remember


### API Usage

To use start the server executable and send a `POST` request to `http://localhost:8888/`.

#### Endpoint

*   `POST /`

#### Request Body

The request must be a JSON object with two keys: `layout` and `weights`.

1.  **`layout`** (string): A 30-character string representing a 3x10 keyboard layout, read from left to right, top to bottom.
2.  **`weights`** (object): A JSON object containing the weights for the statistics you want to evaluate.

The available statistics for weighting are:
*   `sfb`: Same Finger Bigrams
*   `sfs`: Same Finger Skipgrams
*   `lsb`: Index Finger Stretch Bigrams
*   `alt`: Trigram Alternations
*   `rolls`: Trigram Rolls

#### Example Request

Here is an example using `curl`:

```bash
curl -X POST -H "Content-Type: application/json" \
-d '{
  "layout": "qwertyuiopasdfghjklzxcvbnm,.;'",
  "weights": {
    "sfb": -1.5,
    "sfs": -0.7,
    "lsb": -0.8,
    "alt": 0.2,
    "rolls": 0.3
  }
}' \
http://localhost:8888/
```

#### Example Response

The server will respond with a JSON object containing the raw, unweighted percentage for each statistic (`stat_values`) and the final weighted `score`.

```json
{
  "stat_values": {
    "sfb": 4.8312,
    "sfs": 1.1098,
    "lsb": 0.4311,
    "alt": 6.3321,
    "rolls": 5.7812
  },
  "score": -7.5318
}
```
