<h1 align="center">Eve</h1> <br>
<p align="center"> <img src="https://user-images.githubusercontent.com/25558240/46390192-3b7c4d80-c69c-11e8-860e-8b80a04b4072.png" | width=200> </p>
<p align="center">
    <code>eve</code> is your modern-day Navajo code talker
</p>

## Table of Contents
- [Table of Contents](#table-of-contents)
- [Couldn't You Have Done This in Python?](#couldnt-you-have-done-this-in-python)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [License](#license)

<a name="couldnt-you-have-done-this-in-python"></a>
## Couldn't You Have Done This in Python?
[Maybe...](https://youtu.be/7JYJhWIwGUw?t=5)

<a name="start"></a>
## Getting Started

<a name="usage"></a>
## Usage
```
$ eve --help

Decode encrypted or encoded text.

  Usage:
       eve [options] <mode>[=ciphertext]

  Cryptography options:
       -s, --shift     Specify shift(s) to be used
           --caesar    Decode shift ciphers
  
  General options:
       -i, --infile    Specify file to be read
       -o, --outfile   Specify file to be written to
       -h, --help      Show this help menu
           --version   Show program version
```
* For each cipher, there may be a flag that must be set (e.g., shift for Caesar, key for Vigenère, etc.).
* Due to the way Argp parses optional command-line arguments, all STDIN ciphertext specifications must be of the format `--<mode>=CIPHERTEXT`.

To try 2 shifts for a shift cipher:
```
$ eve -s 1,13 --caesar="Uryyb Jbeyq!"
Tqxxa Iadxp!
Hello World!
```

To try all shifts:
```
$ eve -s all --caesar=abc
zab
yza
xyz
wxy
...
```

To specify an input file:
```
$ cat plaintext.txt
one, TWO, 3

$ eve --infile plaintext.txt -s 13 --caesar
bar, GJB, 3
```


<a name="license"></a>
## License
All files within this repo are released under the [MIT (OSI) License](https://en.wikipedia.org/wiki/MIT_License) as per the [LICENSE](https://github.com/bl0nd/architect/blob/master/LICENSE.txt) file stored in the root of this repo.