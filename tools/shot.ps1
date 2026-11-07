Add-Type -AssemblyName System.Drawing
Add-Type @"
using System;
using System.Runtime.InteropServices;
public class W {
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool GetClientRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr dc, uint flags);
  [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr h);
  public struct RECT { public int Left, Top, Right, Bottom; }
}
"@
$dir = "C:\OpenCode-Drive\SharedF\nectariswin\game"
$p = Start-Process -FilePath "$dir\Nec.exe" -WorkingDirectory $dir -PassThru
Start-Sleep -Seconds 3
$p.Refresh()
if ($p.HasExited) { "EXITED code=$($p.ExitCode)"; exit }
$h = $p.MainWindowHandle
"hwnd=$h visible=$([W]::IsWindowVisible($h))"
$r = New-Object W+RECT
[void][W]::GetWindowRect($h, [ref]$r)
$cr = New-Object W+RECT
[void][W]::GetClientRect($h, [ref]$cr)
"window rect = $($r.Left),$($r.Top) - $($r.Right),$($r.Bottom)  size=$($r.Right-$r.Left)x$($r.Bottom-$r.Top)"
"client size = $($cr.Right-$cr.Left)x$($cr.Bottom-$cr.Top)"
$w = $r.Right-$r.Left; $ht = $r.Bottom-$r.Top
if ($w -le 0 -or $ht -le 0) { "bad rect"; Stop-Process -Id $p.Id -Force; exit }
$bmp = New-Object System.Drawing.Bitmap($w, $ht)
$g = [System.Drawing.Graphics]::FromImage($bmp)
$hdc = $g.GetHdc()
$ok = [W]::PrintWindow($h, $hdc, 2)
$g.ReleaseHdc($hdc)
$g.Dispose()
if (-not $ok) {
  $g2 = [System.Drawing.Graphics]::FromImage($bmp)
  $g2.CopyFromScreen($r.Left, $r.Top, 0, 0, (New-Object System.Drawing.Size($w, $ht)))
  $g2.Dispose()
}
$out = "C:\OpenCode-Drive\SharedF\nectariswin\shot1.png"
$bmp.Save($out, [System.Drawing.Imaging.ImageFormat]::Png)
$bmp.Dispose()
"saved $out"
Stop-Process -Id $p.Id -Force
