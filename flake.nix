{
  description = "HackUPC 2026";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-25.11";
  };
  outputs =
    { self, nixpkgs }:
    let
      supportedSystems = [ "x86_64-linux" ];
      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
      pkgs = (import nixpkgs { system = "x86_64-linux"; });
    in
    {
      devShells = forAllSystems (system: {
        default = pkgs.callPackage ./shell.nix { inherit pkgs; };
      });
    };
}
