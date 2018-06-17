bim
===

### Bc IMprooved.

bim takes the idea from bc (linux command line basic calculator) and shift it in to a modern context. bim is designed to evolve into a modern scripting language.


## HOWTO INSTALL

```
$ make help             #(show make options)
$ make                  #(make FLEX=osx #to build in macosx)
$ sudo make install
```

## Dependencies

* flex
* bison
* readline (libreadline libreadline-dev)
* gcc

### documentation

read the [bim man page](doc/xbim.man.html)

### the language

#### comments

```
#this is a comment
```

#### data types

```
2                                #number
0xfe                             # hex number
0b1010                           #binary
2.2                              #number
"foo"                            #interpolated string
'foo'                            #simple string
[1,2,3,4]                        #list
[0x23,"foo",12]                  #list
{'foo':2,'bar':3}                #hash
{foo:2,bar:3}                    #hash
{'index1':"foo",'index2':'foo2'} #hash
{index1:"foo",index2:'foo2'}     #hash

```

#### variables assigning and accessing

```
a="foo"             #a = string 'foo'
a[1]                # 'o'
a=2+2               # 4
a='foo'             # 'foo'
a[0]                # 'f'
a=0xfe              #254
a=[1,2,3,4,5,6]     #a is a list
count(a)            # 6 (number of elements)
a[3]                # 4
a[-1]               # 6
a[2:5]              # [3,4,5,6]
a[:3]               # [1,2,3,4]
a[3:]               # [4,5,6]
a={'foo':2,'bar':3} # a is a hash
a={foo:2,bar:3}     # a is a hash (keys without string quotes)
count(a)            # 2 (number of elements)
a['foo']            # 2
a.foo               # 2
^a=4				# assign global var a (^ refers to global vars ... see functions section).

```
#### arithmetic and boolean opertions

```
2+2     # 4
2+3.1   # 5.1
2-2     # 0
2--2    # 4 ... 2-(-2) :)
2---2   # 0 ... 2-(-(-2))
2+3*4   # 14
(2+3)*4 # 20
6/3     # 2
6/5     # 1.2
#t      # true
#f      # false
!#t     # false (not true)
!#f     # true  (not false)
2<3     # #t
2==2    # #t
2==3    # #f
2>3     # #f
2<=3    # #t
2>=3    # #f
2!=3    # #t
2!=2    # #f
!(2<3)  # #f


```
#### conditionals

```
if(#t) #t else #f
if(2!=2) {prn "this is not executed\n"; prn "foo"} else {prn "multiple commands can be wrapped in block code curly brackets"}

```
#### looping

```
a=1;
while(a<=10){
	prn a,"\n";
	a=a+1;
}

```
#### output

```
prn "list params...",1,2,3,"foo","\n";
print("list params...",1,2,3,"foo","\n");
```

#### functions

```
\x.x+1                   #lambda (anonymous function)
\x.x+1->(3)              #lambda function with application. returns 4
\x.{x+1}->(3)            #same
\x,y.{x+y}->(2,3) #5
\x.{y=3;x+y;prn "done\n";}->(2)
f=\x.x+1                 #assign function to f
f->(3)                   # 4 (function application)
f(3)                     # 4 (shortcut for func apply)
f:\x.x+1                 # you ca assign a function either with '=' or ':'
f->(3)                   # 4 (func application)
f(3)                     # 4 (shortcut for func apply)
f:\_.prn "the _ ignore the parameter" 
                         # if you want to ignore a var you ca use the '_' unsderscore sign as a var name
f()                      # func application with no params
y=5;\x.{x+^y}->(2)       #7 (^ refers to global var)
y=5;\x.{y=1;x+y+^y}->(2) #8 

f:\x.{if(x==1) 1 else x*f(x-1)} #fibonacci func
f(5)                     # 120

```

#### debug

in the repl you can change the var of `__debug__` var to `#t` to debug and inspect internal sturctures.

#### last eval var

the last evaluated expression is stored in the special variable `$!` and you can use it in any way as a normal var in an expression.


### TODO

 * lambda recursion
 * var namespaces
 * IO socket
 * IO files/disk
 * JSON encode/decode
 * DB SQL interact
 * foreach - list and hash loop

#### LICENSE

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
    
