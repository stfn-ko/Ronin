# Types 
## Primitive
  - bool
  - char
  - float
    - f64 
    - f32
  - integer
    - i128
    - i64
    - i32
    - i16
    - i8
  - unsigned integer
    - u128
    - u64
    - u32
    - u16 
    - u8
  - size
    - usize
    - isize 
## Containers
  - tup
  - arr
  - vec 
  - str
## Advanced
  - ptr
  - ref
  - cast
  - never

# Function calls
### with return type in the begining 
```
i32 add_numbers(a:i32, b:i32) {}

i32 add_numbers<a: i32, b: i32> {}
```

### whith special word and return type in the end
```
cast add_numbers(a/i32, b/i32) -> i32 {}

cast add_numbers<a/i32, b/i32> -> i32 {}
```

### with level separation and return type in the begining
```
nov::cast(i32) add_numbers(a/i32, b/i32) {}

nov::cast<i32> add_numbers<a/i32, b/i32> {}
```

