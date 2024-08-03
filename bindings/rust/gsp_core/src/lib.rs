//pub mod gsp_window;

#![allow(non_camel_case_types)]

extern crate libc;
extern crate libloading;

use libc::{c_char, uintptr_t};
use libloading::{Library, Symbol};
use std::ffi::c_void;
use std::sync::Mutex;

pub type gwindow_t = uintptr_t;
//pub type gwindow_event_callback_t = extern "C" fn(gwindow_t, gwindow_event_t);

pub struct GSPCore {
    library: Box<Library>,
    gsp_window_poll_events: unsafe extern "C" fn(),
    gsp_window_create_window: unsafe extern "C" fn() -> gwindow_t
}

// Mutex to safely access the dynamically loaded library
lazy_static::lazy_static! {
    static ref GSP_CORE: Mutex<Option<GSPCore>> = Mutex::new(None);
}

impl GSPCore {
    // Function to load the library and its symbols
    pub fn load(library_path: &str) -> Result<(), Box<dyn std::error::Error>> {
        let lib = unsafe { 
            Library::new(library_path).unwrap()
        };
        
        // Safety: We assume the symbols are valid and present in the library
        unsafe {
            let gsp_window_poll_events: Symbol<unsafe extern "C" fn()> = lib.get(b"gsp_window_poll_events").unwrap();
            let gsp_window_create_window: Symbol<unsafe extern "C" fn() -> gwindow_t> = lib.get(b"gsp_window_create_window").unwrap();

            *GSP_CORE.lock().unwrap() = Some(GSPCore {
                gsp_window_poll_events: *gsp_window_poll_events,
                gsp_window_create_window: *gsp_window_create_window,
                library: Box::new(lib)
            });
        }
        Ok(())
    }

    // Function to register the callback
    pub fn poll_events() {
        unsafe {
            if let Some(ref gsp) = *GSP_CORE.lock().unwrap() {
                (gsp.gsp_window_poll_events)();
            }
        }
    }

    // Function to trigger the callback
    pub fn create_window() -> gwindow_t {
        unsafe {
            if let Some(ref gsp) = *GSP_CORE.lock().unwrap() {
                return (gsp.gsp_window_create_window)();
            } else {
                return 0;
            }
        }
    }
}

pub fn gspcore_init() {
    // Load the library
    GSPCore::load("GSPCore.dll").expect("Failed to load GSPCore.dll");
}

pub fn gspcore_poll() {
    // Load the library
    GSPCore::poll_events();
}

pub fn create_window() -> gwindow_t {
    // Load the library
    return GSPCore::create_window();
}