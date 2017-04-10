# Introduction Algorithms Data Structures Project (2014-2015)

### Contributors
- [@joaobernardo17](https://github.com/joaobernardo17) - João Bernardo Alves
- [@PLopes96](https://github.com/PLopes96) - Pedro Lopes

### About
This project has two parts:
  1. The first part of the project is to develop a program that records and manipulates information regarding the appearance and disappearance of links between banks.
  1. The second part is to record the issue of checks, processing them, and reconciles the accounts of the interveners after this processing.

### How to run:
- `gcc -ansi -Wall -pedantic -o ProjectPart1 ParteI.c`
- `./ProjectPart1`

After starting the program, use the below commands.

### Commands Part 1
- Add a new bank - **a name classification reference**
  - name: Bank name (Maximum of 40 characteres)
  - classification: _1_ (good bank) or _0_ (bad bank)
  - reference: Bank reference (positive integer)
- Give bad classification to a bank - **k reference**
  - reference: Bank reference
- Give good classification to a bank - **r reference**
  - reference: Bank reference
- Add loan - **e reference1 reference2 value**
  - reference1: Reference of the bank that provides the credit
  - reference2: Reference of the bank receiving the loan
  - value - value (positive integer) borrowed
- Add loan amortization - **p reference1 reference2 value**
  - reference1: Reference of the paying bank
  - reference2: Reference of the bank receiving the value
  - value: value (positive integer, less than or equal to the amount owed) paid
- List all banks sorted by system entry providing the information below - **l 0**
  - Bank reference
  - Bank name
  - Bank classification
- List all banks sorted by system entry providing the information below - **l 1**
  - Bank reference
  - Bank name
  - Bank classification
  - Total number of partner banks which the bank has a debt
  - Total number of partner banks whom the bank has borrowed money
  - Total amount lent by the bank in question to other banks
  - Total amount lent by the bank in question to _bad banks_
  - Total amount lent to the bank in question by other banks
  - Total amount lent to the bank in question by _bad banks_
- List the distribution _d(k)_ of the number of banks with exactly _k_ commercial partners - **l 2**
- Demote _good bank_ in greater difficulties - **k**
- Exit the program - **x**

### Commands Part 2
- Add a new check to the check pool to process - **cheque value reference1 reference2 reference3**
  - value: Check value (positive integer)
  - reference1: Reference of the issuing customer (long integer)
  - reference2: Beneficiary customer reference (long integer)
  - reference3: Check reference (long integer)
- Process the oldest issued check and remove that check from the check pool to process - **processa**
- Process the check and remove that check from the check pool to process - **processaR reference**
  - reference: Check reference
- Print check information - **infocheque reference**
  - reference: Check reference
- Print client information - **infocliente reference**
  - reference: Client reference
- Print all active clients information - **info**
- Exit the program and clear all system information - **sair**

###### P.S. To run tests you need to compile. After compile give an input (_test*.in_) to the program and redirect the output to a file (_test*.txt_) - `./ProjectPart1 < ./Testes\ Publicos/test*.in > ./Testes\ Publicos/test*.txt`
