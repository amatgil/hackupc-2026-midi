{
  pkgs ? import <nixpkgs> { },
  lib,
}:
let
  packages = with pkgs; [
    gcc
    gnumake

    # X11 dependencies
    xorg.libX11
    xorg.libX11.dev
    xorg.libXcursor
    xorg.libXcursor.dev
    xorg.libXi
    xorg.libXi.dev
    xorg.libXinerama
    xorg.libXinerama.dev
    xorg.libXrandr
    xorg.libXrandr.dev
    libxrandr
    libxrandr.dev
    libxinerama
    libxinerama.dev
    #emscripten # web support
    clang
    pkg-config

    raylib
    glfw
  ];
in
pkgs.mkShell {
  # Get dependencies from the main package
  nativeBuildInputs = packages;
  buildInputs = packages;
  env = {
    X11_X11_INCLUDE_PATH = pkgs.xorg.libX11.dev;
    X11_LIB_PATH = pkgs.xorg.libX11.dev;
    LIBCLANG_PATH = "${pkgs.libclang.lib}/lib";
    LD_LIBRARY_PATH = "${lib.makeLibraryPath packages}";
  };
  shellHook = ''
    export CMAKE_PREFIX_PATH=${pkgs.raylib}/lib/cmake:$CMAKE_PREFIX_PATH
  '';
}
