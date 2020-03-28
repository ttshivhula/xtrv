# xtrv

A simple archiving utility

## What is xtrv & unxtrv?

xtrv creates archive files and unxtrv extracts archive files.

## Compile

### From source

1. Clone this repository
2. Compile and install with

```
make && sudo make install
```

## How to use?

### xtrv

#### Compress Only

```
./xtrv file1 dir1 ... -o file.xtrv
```

By default if you don't specify output file `out.xtrv` will be used.

#### Compress and Protect

```
./xtrv file1 dir1 ... -o file.xtrv -k secretkey
```

### unxtrv

#### Unprotected Files

```
./unxtrv file.xtrv
```

#### Protected Files

```
./unxtrv -k secretkey file.xtrv
```

#### List Files Only

```
./unxtrv -l file.xtrv
```
