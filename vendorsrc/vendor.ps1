[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

function Get-Package {
    param(
        [string]$packagename,
        [string]$url,
        [string]$dirname
    )
    $dir = "C:\libRETS\vendorsrc\{0}\" -f $dirname
    $zip = "{0}{1}.zip" -f $dir, $dirname

    if (!(Test-Path -Path $dir)) {
        New-Item -ItemType directory -Path $dir
    }
    Write-Host ("Downloading {0}... " -f $packagename) -NoNewline
    (New-Object System.Net.WebClient).DownloadFile($url, $zip)
    Write-Host "DONE"
}

function Extract-Package {
    param(
        [string]$packagename,
        [string]$dirname
    )

    $dir = "C:\libRETS\vendorsrc\{0}\" -f $dirname
    $zip = "{0}{1}.zip" -f $dir, $dirname
    $cur = "{0}current" -f $dir
    
    $sevenzip = "C:\Program Files\7-Zip\7z.exe"

    if (Test-Path -Path $cur) {
        Remove-Item -Path $cur -Recurse -Force
    }
    [String[]]$prefiles = Get-ChildItem -Path $dir -Name -Attributes Directory
    Write-Host ("Extracting {0}... " -f $packagename) -NoNewline
    Invoke-Expression "& '$sevenzip' x '$zip' -o'$dir'"
    [String[]]$postfiles = Get-ChildItem -Path $dir -Name -Attributes Directory
    [String]$exdir = $postfiles | Where-Object { $prefiles -notcontains $_ }
    Rename-Item -Path ("{0}{1}" -f $dir, $exdir)  $cur
    Write-Host "[DONE]"
}

function Extract-TarGz-Package {
    param(
        [string]$packagename,
        [string]$dirname
    )

    $dir = "C:\libRETS\vendorsrc\{0}\" -f $dirname
    $tar = "{0}{1}.tar" -f $dir, $dirname
    $gz = "{0}.gz" -f $tar
    $cur = "{0}current" -f $dir

    $sevenzip = "C:\Program Files\7-Zip\7z.exe"
    
    if (Test-Path -Path $cur) {
        Remove-Item -Path $cur -Recurse -Force
    }
    [String[]]$prefiles = Get-ChildItem -Path $dir -Name -Attributes Directory
    Write-Host ("Extracting {0}... " -f $packagename) -NoNewline
    Invoke-Expression "& '$sevenzip' x '$gz' -o'$dir'"
    Invoke-Expression "& '$sevenzip' x '$tar' -o'$dir'"
    [String[]]$postfiles = Get-ChildItem -Path $dir -Name -Attributes Directory
    [String]$exdir = $postfiles | Where-Object { $prefiles -notcontains $_ }
    Rename-Item -Path ("{0}{1}" -f $dir, $exdir)  $cur
    Write-Host "[DONE]"
}

function Get-OpenSSL {
    param([string]$version)
    $url = "https://github.com/openssl/openssl/archive/OpenSSL_{0}.zip" -f ($version -replace "\.", "_")
    Get-Package "OpenSSL" $url "openssl"
    Extract-Package "OpenSSL" "openssl"
}

function Get-Curl {
    param([string]$version)
    $url = "https://github.com/curl/curl/archive/curl-{0}.zip" -f ($version -replace "\.", "_")
    Get-Package "Curl" $url "curl"
    Extract-Package "Curl" "curl"
}

function Get-Antlr {
    param([string]$version)
    $url = "https://www.antlr2.org/download/antlr-2.7.7.tar.gz"
    Get-Package "ANTLR" $url "antlr"
    Rename-Item -Path "C:\libRETS\vendorsrc\antlr\antlr.zip" "C:\libRETS\vendorsrc\antlr\antlr.tar.gz"
    Extract-TarGz-Package "ANTLR" "antlr"
}

function Get-Boost {
    param([string]$version)
    $url = "https://dl.bintray.com/boostorg/release/{0}/source/boost_{1}.zip" -f $version, ($version -replace "\.", "_")
    Get-Package "Boost" $url "boost"
    Extract-Package "Boost" "boost"
}

function Get-Expat {
    param([string]$version)
    $url = "https://github.com/libexpat/libexpat/archive/R_{0}.zip" -f ($version -replace "\.", "_")
    Get-Package "Expat" $url "expat"
    Extract-Package "Expat" "expat"
}

function Get-Pcre {
    param([string]$version)
    $url = "https://ftp.pcre.org/pub/pcre/pcre-{0}.zip" -f $version
    Get-Package "PCRE" $url "pcre"
    Extract-Package "OpenSSL" "pcre"
}

function Get-Swig {
    param([string]$version)
    $url = "https://github.com/swig/swig/archive/rel-{0}.zip" -f $version
    Get-Package "SWIG" $url "swigwin"
    Extract-Package "OpenSSL" "swigwin"
}

$versions = Get-Content "vendorversions" | ConvertFrom-StringData


#Get-OpenSSL $versions.OpenSSL
Get-Curl $versions.Curl
#Get-Antlr $versions.Antlr
#Get-Boost $versions.Boost
#Get-Expat $versions.Expat
#Get-Pcre $versions.Pcre
#Get-Swig $versions.Swig
