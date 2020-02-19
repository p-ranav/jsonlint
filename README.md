<p align="center">
  <img height="100" src="img/logo.png" alt="jsonlint"/>
</p>

`jsonlint` is a lightweight command-line tool for validating JSON.

## Building `jsonlint`

```bash
$ git clone https://github.com/p-ranav/jsonlint.git
$ cd jsonlint
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

<img src="img/01.png" alt="unterminated string"/>

### Trailing comma in array

<img src="img/02.png" alt="trailing comma"/>

### Duplicate key in object

<img src="img/03.png" alt="duplicate key"/>

### Typo in keyword

<img src="img/04.png" alt="typo in keyword"/>
