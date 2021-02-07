// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Plasma
{

// Utility Functions
namespace ExportUtility
{

// Files or extensions that are NOT automatically copied (may be handled
// specially)
HashSet<String>& GetExcludedFiles()
{
  static HashSet<String> files;
  files.Insert("PlasmaEditor.exe");
  files.Insert("exp");
  files.Insert("ilk");
  files.Insert("lib");
  files.Insert("pdb");
  files.Insert("obj");
  files.Insert("iobj");
  files.Insert("ipdb");
  files.Insert("log");
  return files;
}

void AddFilesHelper(StringParam directory,
                    StringParam relativePathFromStart,
                    HashSet<String>& additionalFileExcludes,
                    FileCallback callback,
                    void* userData)
{
  HashSet<String>& excludedFiles = GetExcludedFiles();

  // Get the total number of files being added to calculate and show progress
  // for the exporting screen
  float i = 0.0f;
  float totalFiles = 0.0f;
  for (FileRange fileRange(directory); !fileRange.Empty(); fileRange.PopFront())
    totalFiles += 1.0f;

  for (FileRange fileRange(directory); !fileRange.Empty(); fileRange.PopFront())
  {
    String fileName = fileRange.Front();

    if (excludedFiles.Contains(fileName) || excludedFiles.Contains(FilePath::GetExtension(fileName)) ||
        additionalFileExcludes.Contains(fileName) || additionalFileExcludes.Contains(FilePath::GetExtension(fileName)))
      continue;

    String newRelativePath = FilePath::Combine(relativePathFromStart, fileName);

    String fullPath = FilePath::Combine(directory, fileName);
    if (DirectoryExists(fullPath))
    {
      AddFilesHelper(fullPath, newRelativePath, additionalFileExcludes, callback, userData);
    }
    else
    {
      callback(fullPath, newRelativePath, fileName, userData, i / totalFiles);
    }
  }
}

void AddFiles(StringParam directory, HashSet<String>& additionalFileExcludes, FileCallback callback, void* userData)
{
  AddFilesHelper(directory, String(), additionalFileExcludes, callback, userData);
}

void ArchiveLibraryOutput(Archive& archive, ContentLibrary* library)
{
  ResourceListing listing;
  library->BuildListing(listing);

  // Add every file in the package to the archive
  String outputPath = library->GetOutputPath();
  String archivePath = library->Name;

  int itemsDone = 0;
  float librarySize = (float)listing.Size();

  forRange (ResourceEntry resource, listing.All())
  {
    ++itemsDone;
    String fullPath = FilePath::Combine(outputPath, resource.Location);
    String relativePath = FilePath::Combine(archivePath, resource.Location);

    // Don't export resources that are marked as template files.
    if (resource.GetResourceTemplate() && resource.Type.Contains("Lightning"))
      continue;

    archive.AddFile(fullPath, relativePath);
    PL::gEngine->LoadingUpdate(
        "Archive Library", library->Name, resource.Name, ProgressType::Normal, float(itemsDone) / librarySize);
  }

  // Finally add the pack file
  String packFile = BuildString(library->Name, ".pack");
  String packFilePath = FilePath::Combine(outputPath, packFile);

  archive.AddFile(packFilePath, FilePath::Combine(archivePath, packFile));
}

void ArchiveLibraryOutput(Archive& archive, StringParam libraryName)
{
  ContentLibrary* library = PL::gContentSystem->Libraries.FindValue(libraryName, nullptr);
  if (library)
    ArchiveLibraryOutput(archive, library);
}

void CopyLibraryOut(StringParam outputDirectory, ContentLibrary* library, bool skipTemplates)
{
  String libraryPath = library->GetOutputPath();
  if (!DirectoryExists(libraryPath))
  {
    PlasmaPrint("Skipped copying library output because it was not built %s\n", library->Name.c_str());
    return;
  }

  String libraryOutputPath = FilePath::Combine(outputDirectory, library->Name);

  CreateDirectoryAndParents(libraryOutputPath);

  // Copy the .pack file
  String packFile = BuildString(library->Name, ".pack");
  String packFileSource = FilePath::Combine(libraryPath, packFile);
  if (FileExists(packFileSource))
  {
    String packFileDestination = FilePath::Combine(libraryOutputPath, packFile);
    CopyFile(packFileDestination, packFileSource);
  }

  BoundType* lightningDocumentType = LightningTypeId(LightningDocumentResource);

  int itemsDone = 0;
  float librarySize = (float)library->GetContentItems().Size();

  forRange (ContentItem* contentItem, library->GetContentItems())
  {
    ++itemsDone;
    bool isTemplate = contentItem->has(ResourceTemplate);

    // Copy each generated Resource
    ResourceListing listing;
    contentItem->BuildListing(listing);
    forRange (ResourceEntry& entry, listing.All())
    {
      // Skip lightning Resource Templates
      if (isTemplate && skipTemplates)
      {
        BoundType* resourceType = MetaDatabase::FindType(entry.Type);

        // Skip lightning resource types
        if (resourceType->IsA(lightningDocumentType))
        {
          continue;
        }
      }

      String fileName = entry.Location;
      String source = FilePath::Combine(libraryPath, fileName);
      if (!FileExists(source))
        continue;

      String destination = FilePath::Combine(libraryOutputPath, fileName);
      CopyFile(destination, source);
      PL::gEngine->LoadingUpdate("Copying", fileName, "", ProgressType::Normal, float(itemsDone) / librarySize);
    }
  }
}

void CopyLibraryOut(StringParam outputDirectory, StringParam name, bool skipTemplates)
{
  ContentLibrary* library = PL::gContentSystem->Libraries.FindValue(name, nullptr);
  if (library)
    CopyLibraryOut(outputDirectory, library, skipTemplates);
}

void RelativeCopyFile(StringParam dest, StringParam source, StringParam filename)
{
  CopyFile(FilePath::Combine(dest, filename), FilePath::Combine(source, filename));
}

void ArchiveFileCallback(
    StringParam fullPath, StringParam relativePath, StringParam fileName, void* userData, float progressPercent)
{
  Archive& archive = *(Archive*)userData;
  archive.AddFile(fullPath, relativePath);
  PL::gEngine->LoadingUpdate("Archiving", fileName, "", ProgressType::Normal, progressPercent);
}

void CopyFileCallback(
    StringParam fullPath, StringParam relativePath, StringParam fileName, void* userData, float progressPercent)
{
  String& outputDirectory = *(String*)userData;
  String destPath = FilePath::Combine(outputDirectory, relativePath);
  CreateDirectoryAndParents(FilePath::GetDirectoryPath(destPath));
  CopyFile(destPath, fullPath);
  PL::gEngine->LoadingUpdate("Copying", fileName, "", ProgressType::Normal, progressPercent);
}

} // namespace ExportUtility

// Content Source
struct ExportTargetSource : public DataSource
{
  ExportTargetList* mExportList;

  ExportTargetSource(ExportTargetList* exportList) : mExportList(exportList)
  {
  }

  DataEntry* GetRoot() override
  {
    return (DataEntry*)mExportList;
  }

  DataEntry* ToEntry(DataIndex index) override
  {
    return (DataEntry*)index.Id;
  }

  DataIndex ToIndex(DataEntry* dataEntry) override
  {
    return DataIndex((u64)dataEntry);
  }

  DataEntry* Parent(DataEntry* dataEntry) override
  {
    return nullptr;
  }

  uint ChildCount(DataEntry* dataEntry) override
  {
    if (dataEntry == mExportList)
      return mExportList->Entries.Size();
    else
      return 0;
  }

  DataEntry* GetChild(DataEntry* dataEntry, uint index, DataEntry* prev) override
  {
    ExportTargetList* listing = (ExportTargetList*)dataEntry;
    return (DataEntry*)listing->SortedEntries[index];
  }

  bool IsExpandable(DataEntry* dataEntry) override
  {
    return dataEntry == mExportList;
  }

  void GetData(DataEntry* dataEntry, Any& variant, StringParam column) override
  {
    // Ignore the root
    if (dataEntry == mExportList)
      return;

    ExportTargetEntry* entry = (ExportTargetEntry*)dataEntry;
    if (!column.Empty())
    {
      if (column == CommonColumns::Name)
      {
        variant = entry->TargetName;
      }
      else if (column == "Export")
      {
        variant = entry->Export;
      }
    }
  }

  bool SetData(DataEntry* dataEntry, AnyParam variant, StringParam column) override
  {
    ExportTargetEntry* entry = (ExportTargetEntry*)dataEntry;
    if (!column.Empty())
    {
      if (column == "Export")
      {
        entry->Export = variant.Get<bool>();
        return true;
      }
    }
    return false;
  }
};

// ExportTargetList
ExportTargetList::ExportTargetList()
{
}

ExportTargetList::~ExportTargetList()
{
  DeleteObjectsInContainer(SortedEntries);
}

void ExportTargetList::AddEntry(ExportTargetEntry* entry)
{
  Entries.Insert(entry->TargetName, entry);
  SortedEntries.PushBack(entry);
}

void ExportTargetList::SetActiveTargets(HashSet<String>& activeTargets)
{
  forRange (ExportTargetEntry* entry, SortedEntries)
  {
    if (activeTargets.Contains(entry->TargetName))
      entry->Export = true;
  }
}

HashSet<String> ExportTargetList::GetActiveTargets()
{
  HashSet<String> activeTargets;
  forRange (ExportTargetEntry* entry, SortedEntries)
  {
    if (entry->Export)
      activeTargets.Insert(entry->TargetName);
  }
  return activeTargets;
}

Exporter::Exporter()
{
  // Add all available export targets
  mExportTargets.Insert("Windows", new WindowsExportTarget(this, "Windows"));
  mExportTargets.Insert("Web", new EmscriptenExportTarget(this, "Web"));
  mDefaultTargets.Insert("Windows");
}

void Exporter::ExportAndPlay(Cog* projectCog)
{
  mProjectCog = projectCog;
  mPlay = true;

  mApplicationName = "GameTest.exe";
  mOutputDirectory = GetTemporaryDirectory();

  ExportApplication(mDefaultTargets);
}

void Exporter::ExportGameProject(Cog* projectCog)
{
  mProjectCog = projectCog;
  mPlay = false;
}

void Exporter::UpdateIcon(ProjectSettings* project, ExecutableResourceUpdater& updater)
{
  // Assume the icon file is located in the project direction with the name
  // "Icon.ico"
  String iconFile = FilePath::Combine(project->ProjectFolder, "Icon.ico");
  if (FileExists(iconFile))
  {
    // Read the file into memory
    size_t bufferSize = 0;
    byte* buffer = ReadFileIntoMemory(iconFile.c_str(), bufferSize);

    if (buffer && bufferSize)
      updater.UpdateIcon(buffer, bufferSize);

    plDeallocate(buffer);
  }
}

void Exporter::SaveAndBuildContent()
{
}

void Exporter::ExportApplication(HashSet<String> exportTargets)
{
  SaveAndBuildContent();

  PL::gEngine->LoadingStart();
  CreateDirectoryAndParents(mOutputDirectory);
  forRange (ExportTarget* exportTarget, mExportTargets.Values())
  {
    if (exportTargets.Contains(exportTarget->mTargetName))
      exportTarget->ExportApplication();
  }
  PL::gEngine->LoadingFinish();
  Download(mOutputDirectory);
}

void Exporter::ExportContent(HashSet<String> exportTargets)
{
  SaveAndBuildContent();

  PL::gEngine->LoadingStart();
  CreateDirectoryAndParents(mOutputDirectory);
  forRange (ExportTarget* exportTarget, mExportTargets.Values())
  {
    if (exportTargets.Contains(exportTarget->mTargetName))
      exportTarget->ExportContentFolders(mProjectCog);
  }
  PL::gEngine->LoadingFinish();
  Download(mOutputDirectory);
}

void Exporter::CopyContent(Status& status, String outputDirectory, ExportTarget* target)
{
  Assert(target, "A valid export target should always be provided");

  ProjectSettings* project = mProjectCog->has(ProjectSettings);
  String appDirectory = GetApplicationDirectory();
  Cog* configCog = PL::gEngine->GetConfigCog();
  MainConfig* mainConfig = configCog->has(MainConfig);

  // Delete the old content if it was previously exported
  bool directoryEmpty = EnsureEmptyDirectory(outputDirectory);
  if (!directoryEmpty)
  {
    status.SetFailed("Unable to delete the output directory contents");
    return;
  }

  // Copy content output
  ExportUtility::CopyLibraryOut(outputDirectory, "FragmentCore");
  ExportUtility::CopyLibraryOut(outputDirectory, "Loading");
  ExportUtility::CopyLibraryOut(outputDirectory, "PlasmaCore");
  ExportUtility::CopyLibraryOut(outputDirectory, "UiWidget");
  ExportUtility::CopyLibraryOut(outputDirectory, "EditorUi");
  ExportUtility::CopyLibraryOut(outputDirectory, "Editor");
  ExportUtility::CopyLibraryOut(outputDirectory, project->ProjectContentLibrary);

  forRange (ContentLibrary* library, project->ExtraContentLibraries.All())
  {
    ExportUtility::CopyLibraryOut(outputDirectory, library);
  }
  
  // Once the build output is separated by platform this should not be needed
  HashSet<String>& additionalExcludes = target->GetAdditionalExcludedFiles();

  // Copy default configuration
  //ExportUtility::RelativeCopyFile(outputDirectory, appDirectory, "Configuration.data");

  // Copy Inno Setup Template
  target->CopyInstallerSetupFile(
      outputDirectory, mainConfig->DataDirectory, project->ProjectName, project->GetProjectGuid());

  // Add all dlls (and other files next to the exe)
  ExportUtility::AddFiles(appDirectory, additionalExcludes, ExportUtility::CopyFileCallback, &outputDirectory);

  // Copy the project file
  CopyFile(FilePath::Combine(outputDirectory, "Project.plasmaproj"), project->ProjectFile);

  // Add data directory
  String dataDirectory = mainConfig->DataDirectory;
  ExportUtility::AddFilesHelper(
      dataDirectory, "Data", additionalExcludes, ExportUtility::CopyFileCallback, &outputDirectory);
}

} // namespace Plasma
