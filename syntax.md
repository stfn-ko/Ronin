### Variable assignment

**Type/Ownership variable = value;**

Ownership can be of two types: `r` and `rw`, which stand for read-only & read-write.

### Examples

    `i32/rw variable = 10;`

    `f64/r variable = 3.14;`

    `bool/rw variable = true;`

    `char/r variable = 'A';`

    `str/rw variable = "Hello";`

    `MyCustomType/r variable = createCustomType();`

### Control Flow

### Conditional Statements (if-else)

> ℹ️ can avoid ‘{}’ if it is a one-liner

**`if (statement) {…}`**

**`else if (statement) {…}`**

**`else {…}`**

### Loop Contructs

### For Loops

### Option 1

**`for (i32/rw i = 0; i < 5; ++i) {…}`**

This for loop follows a traditional C++-style syntax. It initializes a mutable **`i32`** variable named **`i`** to 0. The loop executes as long as the condition **`i < 5`** is true. After each iteration, the value of **`i`** is incremented by 1 (**`++i`**). This loop allows you to have explicit control over the loop variable and is commonly used when you need to iterate a specific number of times.

### Option 2

**`for (i32/r &val,it : arr) {…}`**

This for loop uses a range-based iteration syntax. It iterates over the elements of the **`arr`** array. The **`val`** variable represents an immutable reference to each element of the array. The **`it`** variable represents the iteration count or index. This loop allows you to access each element of the array using **`val`** and the iteration count using **`it`**. It is useful when you need both the element value and its corresponding index during the loop execution.

### Option 3

**`for (i32/r &val : arr) {…}`**

This for loop is similar to the previous example but does not include the iteration count or index. It iterates over the elements of the **`arr`** array and assigns each element's value to the immutable reference variable **`val`**. This loop is convenient when you only need to access the values of the array elements during the loop execution.

### While Loops

**`while (i < 10) {…}`**

### Infinite Loops

**`loop {…}`**

### Loop Control Statements

**`break` breaks from any loop**

**`skip` skips the rest of the current iteration**

### Branching Constructs

### Match

> ℹ️ can avoid ‘{}’ if it is a one-liner

```
match (expression) {
    pattern1 :: {
        // Code block 1
    }
    // Add more patterns as needed
    _ :: {
        // Default code block
    }
}
```

### Patterns

### Variable Binding

```
i32/r i = 10;
match i {
    0 := i32/r x :: {
        println(x); //prints 0
    }
    10 :: {
        i32/rw x = 20;
        println(x); //prints 20
    }
    _ :: {
        println("Foo");
    }
}
```

In this example, the first pattern **`0 := i32/r x`** matches when **`i`** is equal to 0. It uses the **`:=`** operator for forward assignment, assigning the value of **`i`** to the immutable variable **`x`**. The code block associated with this pattern is executed, and inside the code block, **`x`** is printed, resulting in the output **`0`**.

The second pattern **`10 ::`** matches when **`i`** is equal to 10. It does not require any variable assignment. Instead, a new mutable variable **`x`** of type **`i32`** is declared within the code block with an initial value of 20. The code block associated with this pattern is executed, and inside the code block, **`x`** is printed, resulting in the output **`20`**.

The last pattern **`_ ::`** acts as a catch-all pattern, matching any value that did not match the previous patterns. In this case, the code block associated with the catch-all pattern simply prints the string "Foo" to the console.

> ℹ️ In Sorcero, the `:=` operator can be used as an alternative syntax to initialize a variable with a value at the time of declaration. Logically `i32/r x = 10` is equal to `10 := i32/r x`

### Multiple Patterns

In Sorcero, you can utilize multiple patterns in a **`match`** statement to handle different cases based on the value being matched. To achieve this, you can separate multiple patterns using the **`|`** operator.

Here's an example that demonstrates multiple pattern matching in Sorcero:

```
i32/r i = 10;
match i {
    0 | 1 :: {
        println(i); //prints 0 or 1
    }
    2 .. 10 :: {
        println(i); //prints anything from 2 up to 9
    }
    10 ..= 20 :: {
        println(i); //prints anything from 10 up to 20
    }
    _ :: {
        println("Foo");
    }

}
```

The cases **`0`** and **`1`** are combined using the **`|`** operator. Similarly, the pattern **`2 .. 10`** matches any value from **`2`** to `**9**` (exclusive), and **`10 ..= 20`** matches any value from **`10`** to **`20`** (inclusive).
