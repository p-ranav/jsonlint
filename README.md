<p align="center">
  <img height="100" src="https://i.imgur.com/tBgn9t9.png" alt="jsonlint"/>
</p>

`jsonlint` is a lightweight command-line tool for validating JSON.

## Building `jsonlint`

```bash
$ git clone https://github.com/p-ranav/jsonlint.git
$ cd tsparse
$ mkdir build && cd build
$ cmake .. && make && make check
```

## Usage

```bash
$ ./jsonlint --help
Usage: jsonlint [options] file 

Positional arguments:
file      	json file to validate

Optional arguments:
-h --help 	show this help message and exit
```

## Example Scenarios

### Unterminated String

<img src="https://i.imgur.com/CraG7PY.png" alt="unterminated string"/>

### Trailing comma in array

<img src="https://i.imgur.com/dWTPSns.png" alt="trailing comma"/>

### Duplicate key in object

<img src="https://i.imgur.com/Kgee8Ak.png" alt="duplicate key"/>

### Typo in keyword

<img src="https://i.imgur.com/BOon2Q4.png" alt="typo in keyword"/>
