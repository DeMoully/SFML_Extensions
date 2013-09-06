The Forest Library is a suite of tools designed to extend the usability of SFML. Below you will find a brief description of each tool, along with some basic information about its capabilities.

Animation.hpp - this header file defines the Animation class, which is a member of the sfext namespace. This class is similar to the sf::Sprite class and contains most of the functions available to an sf::Sprite. Additional functionalities include the ability to draw at a specific position, to draw a specific frame, or to draw at a specific time. Internally, the class uses an oak::FlexibleClock to keep track of time. This will eventually allow the user to speed up the animation, reverse it, etc.

AnimationHandler.hpp - this header file defines the AnimationHandler class, which is a member of the sfext namespace. This class stores Animations and has functions for batching.

FileWrapper.hpp - this header file wraps the functionalities of std::fstream in a simple and easy to use class.

FlexibleClock.hpp - this header file defines the FlexibleClock class, which is a member of the oak namespace (which deals with math and math-related classes). It can be paused, sped up, slowed down, etc.

FontHandler.hpp - this header file defines the FontHandler class, which maps sf::Strings to sf::Fonts and is a member of the sfext namespace. This keeps the user from needing to keep track of fonts themselves and is used internally in the sfext::TextHandler.

ForwardRange.hpp - this header file defines the ForwardRange class, which is a member of the oak namespace. This class can be used with C++11 range based for loops. It is similar to Python's range(), but only iterates forward by 1 unit each step.

Random.hpp - this header file defines several functions associated with random number generation and is contained within the oak namespace.

Range.hpp - this header file defines the Range class, which is a member of the oak namespace. It is a generic version of the ForwardRange class, and can be iterated in either an increasing or decreasing manner.

SpriteHandler.hpp - this header file defines the SpriteHandler class, which is a member of the sfext namespace. It maps sf::Strings to sf::Sprites, which helps to prevent unnecessary resource use.

TextHandler.hpp - this header file defines the TextHandler class, which is a member of the sfext namespace. It allows users to easily write things to a RenderTarget at any position. It stores a map of fonts to allow users to switch fonts quickly and easily.

Tweener.hpp - this header file defines two classes, the Tweener and DiscreteTweener. They are both members of the oak namespace. The Tweener class is a "continuous" tweener and must be asked for its value, while the Discrete Tweener is - as its name suggests - discrete, meaning that it must be updated. It stores a reference to a float, which is updated whenever the update function is called. These classes are not yet finished, so they may or may not contain bugs.

VectorMath.hpp - this header file defines a set of functions, most of which are associated with vector mathematics. All of these functions are located in the oak namespace.