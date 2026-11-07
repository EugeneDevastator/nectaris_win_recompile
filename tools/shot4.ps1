Add-Type -AssemblyName System.Drawing
Add-Type @"
using System;
using System.Runtime.InteropServices;
public class W4 {
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr dc, uint flags);
  [DllImport("user32.dll")] public static extern bool PostMessage(IntPtr h, uint msg, IntPtr w, IntPtr l);
  public struct RECT { public int Left, Top, Right, Bottom; }
  public static void Key(IntPtr h, byte vk){
     PostMessage(h,0x0100,(IntPtr)vk,(IntPtr)1); System.Threading.Thread.Sleep(70);
     PostMessage(h,0x0101,(IntPtr)vk,(IntPtr)0xC0000001);
  }
}
"@
$dir="C:\OpenCode-Drive\SharedF\nectariswin\game"
$p=Start-Process -FilePath "$dir\Nec.exe" -WorkingDirectory $dir -PassThru
Start-Sleep -Seconds 4
$p.Refresh(); if($p.HasExited){"EXITED";exit}
$h=$p.MainWindowHandle
for($i=1;$i -le 6;$i++){
  [W4]::Key($h,0x0D)
  Start-Sleep -Milliseconds 1400
  $p.Refresh()
  $r=New-Object W4+RECT; [void][W4]::GetWindowRect($h,[ref]$r)
  $w=$r.Right-$r.Left; $ht=$r.Bottom-$r.Top
  $bmp=New-Object System.Drawing.Bitmap($w,$ht)
  $g=[System.Drawing.Graphics]::FromImage($bmp); $dc=$g.GetHdc()
  $ok=[W4]::PrintWindow($h,$dc,2); $g.ReleaseHdc($dc)
  # also sample center pixel to detect black
  $px=$bmp.GetPixel([int]($w/2),[int]($ht/2))
  $g.Dispose()
  $bmp.Save("C:\OpenCode-Drive\SharedF\nectariswin\shot_s$i.png",[System.Drawing.Imaging.ImageFormat]::Png)
  $bmp.Dispose()
  "step $i print=$ok rect=$($r.Left),$($r.Top) ${w}x${ht} center=$($px.R),$($px.G),$($px.B) title='$($p.MainWindowTitle)'"
  Start-Sleep -Milliseconds 300
}
Start-Sleep -Milliseconds 300
$p.Refresh(); if(-not $p.HasExited){Stop-Process -Id $p.Id -Force;"killed"}else{"exited $($p.ExitCode)"}
