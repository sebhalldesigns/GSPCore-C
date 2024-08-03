fn main() {

    println!("cargo:rustc-link-search=native=../../../build/lib/Debug");

    // Tell Cargo to link to the `GSPCore` library
    println!("cargo:rustc-link-lib=dylib=GSPCore");

    println!("cargo:rerun-if-changed=../../../build/lib/Debug");

}