Icarus::Icarus() {
  Database::Famicom = BML::unserialize(string::read(locate("database/Famicom.bml")));
  Database::SuperFamicom = BML::unserialize(string::read(locate("database/Super Famicom.bml")));
  Database::MasterSystem = BML::unserialize(string::read(locate("database/Master System.bml")));
  Database::MegaDrive = BML::unserialize(string::read(locate("database/Mega Drive.bml")));
  Database::PCEngine = BML::unserialize(string::read(locate("database/PC Engine.bml")));
  Database::SuperGrafx = BML::unserialize(string::read(locate("database/SuperGrafx.bml")));
  Database::GameBoy = BML::unserialize(string::read(locate("database/Game Boy.bml")));
  Database::GameBoyColor = BML::unserialize(string::read(locate("database/Game Boy Color.bml")));
  Database::GameBoyAdvance = BML::unserialize(string::read(locate("database/Game Boy Advance.bml")));
  Database::GameGear = BML::unserialize(string::read(locate("database/Game Gear.bml")));
  Database::WonderSwan = BML::unserialize(string::read(locate("database/WonderSwan.bml")));
  Database::WonderSwanColor = BML::unserialize(string::read(locate("database/WonderSwan Color.bml")));
  Database::PocketChallengeV2 = BML::unserialize(string::read(locate("database/Pocket Challenge V2.bml")));
  Database::BSMemory = BML::unserialize(string::read(locate("database/BS Memory.bml")));
  Database::SufamiTurbo = BML::unserialize(string::read(locate("database/Sufami Turbo.bml")));
}

auto Icarus::error() const -> string {
  return errorMessage;
}

auto Icarus::missing() const -> string_vector {
  return missingFiles;
}

auto Icarus::success(string location) -> string {
  errorMessage = "";
  return location;
}

auto Icarus::failure(string message) -> string {
  errorMessage = message;
  return {};
}

auto Icarus::manifest(string location) -> string {
  location.transform("\\", "/").trimRight("/").append("/");
  if(!directory_exists(location)) return {};  //change by the libretro team

  auto type = Location::suffix(location).downcase();
  if(type == ".fc") return famicomManifest(location);
  if(type == ".sfc") return superFamicomManifest(location);
  if(type == ".ms") return masterSystemManifest(location);
  if(type == ".md") return megaDriveManifest(location);
  if(type == ".pce") return pcEngineManifest(location);
  if(type == ".sg") return superGrafxManifest(location);
  if(type == ".gb") return gameBoyManifest(location);
  if(type == ".gbc") return gameBoyColorManifest(location);
  if(type == ".gba") return gameBoyAdvanceManifest(location);
  if(type == ".gg") return gameGearManifest(location);
  if(type == ".ws") return wonderSwanManifest(location);
  if(type == ".wsc") return wonderSwanColorManifest(location);
  if(type == ".pc2") return pocketChallengeV2Manifest(location);
  if(type == ".bs") return bsMemoryManifest(location);
  if(type == ".st") return sufamiTurboManifest(location);

  return {};
}

auto Icarus::import(string location) -> string {
  errorMessage = {};
  missingFiles = {};

  location.transform("\\", "/").trimRight("/");
  if(!exists(location)) return failure("file does not exist");  //change by the libretro team
  if(!readable(location)) return failure("file is unreadable");  //change by the libretro team

  auto name = Location::prefix(location);
  auto type = Location::suffix(location).downcase();
  if(!name || !type) return failure("invalid file name");

  auto buffer = read(location);  //change by the libretro team
  if(!buffer) return failure("file is empty");

  if(type == ".zip") {
    Decode::ZIP zip;
    if(!zip.open(location)) return failure("ZIP archive is invalid");
    if(!zip.file) return failure("ZIP archive is empty");

    name = Location::prefix(zip.file[0].name);
    type = Location::suffix(zip.file[0].name).downcase();
    buffer = zip.extract(zip.file[0]);
  }

  if(type == ".fc" || type == ".nes") return famicomImport(buffer, location);
  if(type == ".sfc" || type == ".smc") return superFamicomImport(buffer, location);
  if(type == ".ms" || type == ".sms") return masterSystemImport(buffer, location);
  if(type == ".md" || type == ".smd" || type == ".gen") return megaDriveImport(buffer, location);
  if(type == ".pce") return pcEngineImport(buffer, location);
  if(type == ".sg" || type == ".sgx") return superGrafxImport(buffer, location);
  if(type == ".gb") return gameBoyImport(buffer, location);
  if(type == ".gbc") return gameBoyColorImport(buffer, location);
  if(type == ".gba") return gameBoyAdvanceImport(buffer, location);
  if(type == ".gg") return gameGearImport(buffer, location);
  if(type == ".ws") return wonderSwanImport(buffer, location);
  if(type == ".wsc") return wonderSwanColorImport(buffer, location);
  if(type == ".pc2") return pocketChallengeV2Import(buffer, location);
  if(type == ".bs") return bsMemoryImport(buffer, location);
  if(type == ".st") return sufamiTurboImport(buffer, location);

  return failure("unrecognized file extension");
}

auto Icarus::concatenate(vector<uint8_t>& output, string location) -> void {
  if(auto input = read(location)) {  //change by the libretro team
    auto size = output.size();
    output.resize(size + input.size());
    memory::copy(output.data() + size, input.data(), input.size());
  }
}
