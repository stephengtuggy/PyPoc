param(
    [String]$Generator = "VS2019Win64", # Other options include "ninja" and "VS2022Win64"
    [Boolean]$EnablePIE = $false,
    [String]$BuildType = "Release" # You can also specify "Debug"
)

[String]$cmakePresetName = ""
if ($Generator -ieq "Ninja") {
    $cmakePresetName += "windows-ninja"
} elseif ($Generator -ieq "VS2019Win64") {
    $cmakePresetName += "VS2019Win64"
} elseif ($Generator -ieq "VS2022Win64") {
    $cmakePresetName += "VS2022Win64"
} else {
    Write-Error "Invalid value for Generator: $Generator"
    exit 1
}
$cmakePresetName += "-"
if ($EnablePIE) {
    $cmakePresetName += "pie-enabled"
} else {
    $cmakePresetName += "pie-disabled"
}
$cmakePresetName += "-"
if ($BuildType -ieq "Debug") {
    $cmakePresetName += "debug"
} elseif ($BuildType -ieq "Release") {
    $cmakePresetName += "release"
} else {
    Write-Error "Unrecognized value for BuildType: $BuildType"
    exit 1
}

[String]$baseDir = (Get-Location -PSProvider "FileSystem").Path
[String]$binaryDir = "$baseDir\build\$cmakePresetName"
cmake --preset $cmakePresetName
cmake --build --preset "build-$cmakePresetName" -v
