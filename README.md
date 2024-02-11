![Ronin Logo](/misc/logo_red.jpg)

# Ronin - The Programming Language

## About

The language is still in its early stages of development. When I first started working on this project, my goal was to create something that captures the familiarity of C but diverges from the optimizations introduced in that language due to the technological constraints of its time. While I appreciate feature-rich languages, I dislike the steep learning curve that often accompanies them as newcomers grapple with an overwhelming amount of content.

I aspire for this language to strike a balance between the freedom of C and the robustness of Rust (though perhaps not achieving perfection), infused with some of my unique ideas that have yet to disappoint me (or not). The name "ronin" is inspired by the concept of being a solo warrior, tackling challenges head-on, even as we remain open to future collaborations with like-minded individuals.

## Idea

Ronin will not have classes. It will have so called modules that contain a set of smaller functions, those can be called at any moment with syntax like:

```perl
# In Ronin, every value defaults to a read-only permission.
# Meaning, it is an immutable value
# `i32/r foo` and `i32 foo` are equivalent
# `/r` stands for /read-only
#
# Functions implicitly don't have a return type

mod math {
    fn add(i32 a, i32 b) -> i32 {
        return a + b;
    }

    fn pi() -> f32 {
        return 3.141592653;
    }
}

mod Rectangle {
    @height -> f32;
    @width -> f32;

    fn new(f32 _height, f32 _width) -> Rectangle {
        return Rectangle{.height = _height, .width = _width};
    }

    fn area() -> f32 {
        return @width * @height;
    }

    fn perimeter() -> f32 {
        return 2 * (@width + @height);
    }
}

    ### OUR STUPID COMMENTS ###

main()
{
    Rectangle r1 = Rectangle{.height = 10, .width = 10};
    Rectangle r2 = Rectangle::new(2, 5);

    print(r1::area()); # >> 100
    print(r2::perimeter()); # >> 14
}
```
