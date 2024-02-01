![Ronin Logo](/misc/logo_red.jpg)

# Ronin - The Programming Language

## About

The language is still in its early stages of development. When I first started working on this project, my goal was to create something that captures the familiarity of C but diverges from the optimizations introduced in that language due to the technological constraints of its time. While I appreciate feature-rich languages, I dislike the steep learning curve that often accompanies them as newcomers grapple with an overwhelming amount of content.

I aspire for this language to strike a balance between the freedom of C and the robustness of Rust (though perhaps not achieving perfection), infused with some of my unique ideas that have yet to disappoint me (or not). The name "ronin" is inspired by the concept of being a solo warrior, tackling challenges head-on, even as we remain open to future collaborations with like-minded individuals.

## Idea

Ronin will not have classes. It will have so called parent functions that contain a set of smaller functions, those can be called at any moment with syntax like:

```perl
# In Ronin, every value defaults to a read-only permission.
# Meaning, it is an immutable value
# `i32/r foo` and `i32 foo` are equivalent
# `/r` stands for /read-only
#
# Functions implicitly don't have a return type

math()
{
    add(i32 a, i32 b) -> option::i32
    {
        return as_copy(a + b);
    };

    pi() -> f32
    {
        return 3.14159265358979;
    };
};

math_with_def() -> default()
{
    add(i32 a, i32 b) -> option::i32
    {
        return as_copy(a + b);
    };

    default()
    {
        println("default function");
    }
}

    ### OUR STUPID COMMENTS ###

main()
{
    match [&x] math::add(1,2) {
        some => println("vale: {}", x);
        none => println("no vale");
    }

    math_with_def();
}

            🡣                🡣                🡣

|--> <console/>                                   •  •  •  |
|                                                          |
|  >> "value: 3"                                           |
|                                                          |
|  >> "defaualt function"                                  |
|                                                          |

```
