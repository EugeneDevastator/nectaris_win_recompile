Add-Type -AssemblyName System.Drawing
Add-Type -AssemblyName System.Windows.Forms
Add-Type @"
using System;
using System.Runtime.InteropServices;
public class W2 {
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
  [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr h, int c);
  [DllImport("user32.dll")] public static extern void keybd_event(byte vk, byte scan, uint flags, UIntPtr extra);
  public struct RECT { public int Left, Top, Right, Bottom; }
  public static void Key(byte vk){ keybd_event(vk,0,0,UIntPtr.Zero); System.Threading.Thread.Sleep(60); keybd_event(vk,0,2,UIntPtr.Zero); }
}
"@
$dir = "C:\OpenCode-Drive\SharedF\nectariswin\game"
$p = Start-Process -FilePath "$dir\Nec.exe" -WorkingDirectory $dir -PassThru
Start-Sleep -Seconds 3
$p.Refresh()
if ($p.HasExited) { "EXITED code=$($p.ExitCode)"; exit }
$mods = ($p.Modules | ForEach-Object { $_.ModuleName }) -join ','
"MODULES: $mods"
$h = $p.MainWindowHandle
[void][W2]::ShowWindow($h, 9)
[void][W2]::SetForegroundWindow($h)
Start-Sleep -Milliseconds 500
# press Enter, then wait, screenshot
[W2]::Key(0x0D)
Start-Sleep -Seconds 2
$r = New-Object W2+RECT
[void][W2]::GetWindowRect($h, [ref]$r)
$w=$r.Right-$r.Left; $ht=$r.Bottom-$r.Top
"rect $($r.Left),$($r.Top) ${w}x${ht}"
$bmp = New-Object System.Drawing.Bitmap($w,$ht)
$g=[System.Drawing.Graphics]::FromImage($bmp)
$g.CopyFromScreen($r.Left,$r.Top,0,0,(New-Object System.Drawing.Size($w,$ht)))
$g.Dispose()
$out="C:\OpenCode-Drive\SharedF\nectariswin\shot2.png"
$bmp.Save($out,[System.Drawing.Imaging.ImageFormat]::Png); $bmp.Dispose()
"saved $out"
Start-Sleep -Seconds 1
$p.Refresh()
if(-not $p.HasExited){ "still running"; Stop-Process -Id $p.Id -Force } else { "exited $($p.ExitCode)" }
