/*
 * glutCompat.h
 *
 * Compatibility layer for GLUT functions on different platforms
 * Provides FreeGLUT-like functions for macOS system GLUT
 */

#ifndef GLUT_COMPAT_H_
#define GLUT_COMPAT_H_

#include <cstdlib>  // For exit()

// CRITICAL: GLEW must be included before any OpenGL headers including GLUT
#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>

// macOS system GLUT doesn't have these FreeGLUT functions and constants
// Provide compatibility implementations

#ifndef GLUT_ACTION_ON_WINDOW_CLOSE
#define GLUT_ACTION_ON_WINDOW_CLOSE 0x01F9
#endif

#ifndef GLUT_ACTION_CONTINUE_EXECUTION  
#define GLUT_ACTION_CONTINUE_EXECUTION 0
#endif

// macOS GLUT doesn't define GLUT_KEY_DELETE
#ifndef GLUT_KEY_DELETE
#define GLUT_KEY_DELETE 127
#endif

// Compatibility function for glutSetOption
inline void glutSetOption_compat(GLenum option_flag, int value) {
    // macOS GLUT doesn't support this, but we can safely ignore it
    // The default behavior on macOS is already GLUT_ACTION_CONTINUE_EXECUTION
    (void)option_flag;
    (void)value;
}

// Compatibility function for glutLeaveMainLoop
inline void glutLeaveMainLoop_compat() {
    // On macOS, we use exit to leave the main loop
    // This is less clean but works for the use case
    exit(0);
}

// Define compatibility macros
#define glutSetOption glutSetOption_compat
#define glutLeaveMainLoop glutLeaveMainLoop_compat

#else
// Non-macOS platforms (Linux, Windows with FreeGLUT)
#include <GL/glut.h>
#endif

#endif /* GLUT_COMPAT_H_ */
