
extern crate libc;

use libc::{c_char, uintptr_t};

#[repr(C)]
pub struct gwindow_event_t {
    event_id: u8,
    sub_id: u8,
    data: u64
}

pub type gwindow_t = uintptr_t;
pub type gwindow_event_callback_t = extern "C" fn(gwindow_t, gwindow_event_t);

#[link(name = "GSPCore", kind = "dylib")]
extern {
    pub fn gsp_window_poll_events();
    pub fn gsp_window_is_window_valid(window: gwindow_t) -> bool;
    pub fn gsp_window_create_window() -> gwindow_t;
    pub fn gsp_window_set_title(window: gwindow_t, title: *mut c_char);
    pub fn gsp_window_set_event_callback(window: gwindow_t, event_callback: gwindow_event_callback_t);
}

