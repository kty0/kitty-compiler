{
  description = "Tiger compiler flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-23.11";
    flake-utils.url = "github:numtide/flake-utils";
    buildenv = {
      url =
        "git+https://gitlab.lre.epita.fr/tiger/buildenv?ref=master&dir=nix";
      inputs = { nixpkgs.follows = "nixpkgs"; };
    };
  };

  outputs = { self, nixpkgs, buildenv, flake-utils, ... }:
    let
      systems = [ "x86_64-linux" ];
    in
    flake-utils.lib.eachSystem systems (system:
      let
        inherit (nixpkgs) lib;
        inherit (lib) attrValues;
        pkgs = import nixpkgs { inherit system; };
        defaultStdenv = pkgs.multiStdenv;
        tcPackage =
          let

            buildenvPkgs = with buildenv.outputs.packages.${system}; [
              reflex
            ];

          in
          { lib, pkgs, stdenv }:
          stdenv.mkDerivation {
            pname = "tc";
            version = "1.90";

            src = self;

            enableParallelBuilding = true;
            hardeningDisable = [ "all" ];

            preConfigure = ''
              patchShebangs --build ./build-aux/bin/
              patchShebangs --build ./dev/
              ./bootstrap
            '';

            nativeBuildInputs = with pkgs;
              [
                bison
                libxslt
                autoconf
                automake
                autoreconfHook
                boost
                gnumake
                libtool
                perl
                swig4
                llvmPackages_14.llvm
                pkgs.pkgsi686Linux.llvmPackages_14.clang
              ] ++ buildenvPkgs;


            buildInputs = with pkgs; [ clang-tools_14 doxygen gdb graphviz valgrind libxml2 ];
          };

      in
      {
        defaultPackage = self.packages.${system}.tc;
        packages = {
          tc = pkgs.callPackage tcPackage { stdenv = defaultStdenv; };
          # It does not work currently because nix does not allow 2 Clang in the same env.
          tcClang = pkgs.callPackage tcPackage {
            stdenv = pkgs.llvmPackages_14.stdenv;
          };

          devShell = pkgs.mkShell.override { stdenv = defaultStdenv; } {
            inputsFrom = [ self.packages.${system}.tc ];
          };
        };
      });
}
