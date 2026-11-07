Add-Type -AssemblyName System.Drawing
Add-Type @"
using System;
using System.Runtime.InteropServices;
public class W5 {
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr dc, uint flags);
  [DllImport("user32.dll")] public static extern bool ClientToScreen(IntPtr h, ref POINT p);
  [DllImport("user32.dll")] public static extern bool SetCursorPos(int x,int y);
  [DllImport("user32.dll")] public static extern void mouse_event(uint f,uint dx,uint dy,uint d,UIntPtr e);
  [DllImport("user32.dll")] public static extern void keybd_event(byte vk,byte sc,uint f,UIntPtr e);
  public struct RECT { public int Left,Top,Right,Bottom; }
  public struct POINT { public int X,Y; }
  public static void Key(byte vk){ keybd_event(vk,0,0,UIntPtr.Zero); System.Threading.Thread.Sleep(80); keybd_event(vk,0,2,UIntPtr.Zero); }
  public static void ClickAt(IntPtr h,int cx,int cy){
    POINT p=new POINT(); p.X=cx; p.Y=cy; ClientToScreen(h,ref p);
    SetCursorPos(p.X,p.Y); System.Threading.Thread.Sleep(150);
    mouse_event(0x0002,0,0,0,UIntPtr.Zero); System.Threading.Thread.Sleep(80);
    mouse_event(0x0004,0,0,0,UIntPtr.Zero);
  }
}
"@
$dir="C:\OpenCode-Drive\SharedF\nectariswin\game"
$p=Start-Process -FilePath "$dir\Nec.exe" -WorkingDirectory $dir -PassThru
Start-Sleep -Seconds 4
$p.Refresh(); if($p.HasExited){"EXITED";exit}
$h=$p.MainWindowHandle
$ws=New-Object -ComObject WScript.Shell
$ok=$ws.AppActivate($p.Id)
"AppActivate=$ok"
Start-Sleep -Milliseconds 800
function Snap($n){
  $r=New-Object W5+RECT; [void][W5]::GetWindowRect($h,[ref]$r)
  $w=$r.Right-$r.Left; $ht=$r.Bottom-$r.Top
  $bmp=New-Object System.Drawing.Bitmap($w,$ht)
  $g=[System.Drawing.Graphics]::FromImage($bmp); $dc=$g.GetHdc()
  $ok=[W5]::PrintWindow($h,$dc,2); $g.ReleaseHdc($dc); $g.Dispose()
  $bmp.Save("C:\OpenCode-Drive\SharedF\nectariswin\shot_$n.png",[System.Drawing.Imaging.ImageFormat]::Png); $bmp.Dispose()
  "snap $n print=$ok"
}
[W5]::Key(0x0D); Start-Sleep -Milliseconds 1500; Snap "a1"
[W5]::Key(0x0D); Start-Sleep -Milliseconds 1500; Snap "a2"
# now click START (client approx 190,380)
[W5]::ClickAt($h,190,380); Start-Sleep -Milliseconds 2000; Snap "a3"
Start-Sleep -Milliseconds 1500; Snap "a4"
# click again somewhere center
[W5]::ClickAt($h,320,240); Start-Sleep -Milliseconds 1500; Snap "a5"
$p.Refresh(); if(-not $p.HasExited){Stop-Process -Id $p.Id -Force;"killed"}else{"exited $($p.ExitCode)"}
