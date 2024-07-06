#╔════════════════════════════════════════════════════════════════════════════════╗
#║                                                                                ║
#║   Save-FreeGlutPackage.ps1                                                     ║
#║   save online                                                                  ║
#║                                                                                ║
#╟────────────────────────────────────────────────────────────────────────────────╢
#║   Guillaumep  <guillaumep@luminator.com>                                       ║
#║   Copyright (C) Luminator Technology Group.  All rights reserved.              ║
#╚════════════════════════════════════════════════════════════════════════════════╝



function Save-FreeGlutPackage{
    [CmdletBinding(SupportsShouldProcess)]
    param(
        [Parameter(Mandatory=$False)]
        [string]$Uri = "https://www.transmissionzero.co.uk/files/software/development/GLUT/freeglut-MSVC.zip",
        [Parameter(Mandatory=$False)]
        [string]$Path=""
    )
   try{

        if([string]::IsNullOrEmpty($Path)){
            $Path = "$PWD"
        }
        $FreeGlutUrl = $Uri
        $FileName = ([Uri]$FreeGlutUrl).Segments[([Uri]$FreeGlutUrl).Segments.Count-1] -as [string]
        $TempPath = "$ENV:Temp\{0}" -f ((New-Guid).Guid -as [string])
        $TempFile = "{0}\{1}" -f $TempPath, $FileName
        new-item -path $TempFile -ItemType 'File' -Force -ErrorAction Ignore | Out-Null
        remove-item -path $TempFile -Force -ErrorAction Ignore | Out-Null

        $Res = $Null

        $request = [System.Net.HttpWebRequest]::Create($FreeGlutUrl)
        $request.PreAuthenticate = $false
        $request.Method = 'GET'
        $request.Headers = New-Object System.Net.WebHeaderCollection
        $request.Headers.Add('User-Agent','Mozilla/5.0')


        $request.Headers.Add('Cache-Control', 'no-store')

        $request.set_Timeout(15000)


        $request.CachePolicy                  = New-Object Net.Cache.RequestCachePolicy([Net.Cache.RequestCacheLevel]::NoCacheNoStore)


        [System.Net.WebResponse]$response     = $request.GetResponse()
        [System.IO.Stream]$responseStream     = $response.GetResponseStream()
        [System.IO.FileStream]$targetStream   = [System.IO.FileStream]::new($TempFile, [System.IO.FileMode]::Create)

        $buffer                               = new-object byte[] 10KB
        $count                                = $responseStream.Read($buffer,0,$buffer.length)

        while ($count -gt 0){
            Start-Sleep -Milliseconds 5
           $targetStream.Write($buffer, 0, $count)
           $count = $responseStream.Read($buffer,0,$buffer.length)
        }

        if(Test-Path $TempFile -PathType Leaf){
            $FileSize = (Get-Item $TempFile).Length
            Write-Verbose "Downloaded file `"$TempFile`" ($FileSize bytes)"
            if(-not(Test-Path $Path -PathType Container)){
                Write-Verbose "Creating directory `"$Path`" ($FileSize bytes)"
                new-item -path $Path -ItemType 'Directory' -Force -ErrorAction Ignore | Out-Null
            }
            Write-Verbose "Move-Item `"$TempFile`" --> `"$Path`""
            Copy-Item -Path "$TempFile" -Destination $Path -Force
            Write-Verbose "Remove temp folder `"$TempPath`""
            remove-item -path "$TempFile" -Force -ErrorAction Ignore | Out-Null
        }

        $targetStream.Flush()
        $targetStream.Close()
        $targetStream.Dispose()
        $responseStream.Dispose()

        $Res = Join-Path $Path $FileName
        $Res
    }catch{
        Show-ExceptionDetails ($_) -ShowStack
    }
}

function Save-FreeGlutExternals{
    [CmdletBinding(SupportsShouldProcess)]
    param()  


    try{
        # Script Variables
        
        $ProjectRoot = (Resolve-Path "$PSScriptRoot\..").Path
        $ExternalsPath = (Resolve-Path "$ProjectRoot\externals").Path

        $GlutFile = Save-FreeGlutPackage
        Expand-Archive -Path $GlutFile -DestinationPath "$ExternalsPath" -Force -ErrorAction Ignore

    }catch{
        $ErrorOccured = $True
        Show-ExceptionDetails $_ -ShowStack
    }finally{
        
    }
}

Save-FreeGlutExternals

        
        
    