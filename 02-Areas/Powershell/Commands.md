






```powershell title:""
$source = "C:\源文件夹"
$dest = "C:\目标文件夹"

Get-ChildItem -Path $source -File | ForEach-Object {
    $baseName = $_.BaseName
    $extension = $_.Extension
    $newName = $_.Name
    $counter = 1

    while (Test-Path (Join-Path $dest $newName)) {
        $newName = "{0}-{1}{2}" -f $baseName, $counter, $extension
        $counter++
    }

    Copy-Item -Path $_.FullName -Destination (Join-Path $dest $newName)
}
```




