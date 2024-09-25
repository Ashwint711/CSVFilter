### Features
- Can Display any column from given CSV file.


### Important 
- The input CSV file must contain a "New Line Character" after each, even after last line there must be a New Line Character.

### Usage 

- Compile source code using "make" command and follow below syntax.

####Syntax : 

```
 ./select <Column Name> from <file.csv> where <Col = Val> <Col = Val>
```

####Example : 

```
./select FirstName from students.csv where Degree = MSC Gender = F Year = 2024
```

```
./select LastName from students.csv where Degree = MSC Gender = F Year = 2024
```

```
./select Gender from students.csv where Degree = MSC
```


### Limitations

- Can't fetch more than one column in output.
- Can't use FirstName, MiddleName and LastName as where condition.

