#ifndef GVIEWADAPTIVEPROPERTY_H
#define GVIEWADAPTIVEPROPERTY_H

// GViewAdaptiveProperty is a system that allows variant properties
// to be attached to a view via a 'selector'. There are system
// selectors that can be used such as viewport size and dark mode state.
// But custom selectors can also be defined. 
// These selectors are defined in a way that the triggers are evaluated
// on a system level, so three adaptive properties with triggers as follows:
//      1: viewportWidth > 500
//      2: viewportWidth > 750
//      3: viewportWidth > 500 
//
// would result in 2 trigger evaluations rather than three.





#endif // GVIEWADAPTIVEPROPERTY_H
