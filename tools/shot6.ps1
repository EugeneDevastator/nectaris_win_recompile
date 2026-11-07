Add-Type -AssemblyName System.Drawing
Add-Type @"
using System;
using System.Runtime.InteropServices;
public class W6 {
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr dc, uint flags);
  [DllImport("user32.dll")] public static extern bool ClientToScreen(IntPtr h, ref POINT p);
  [DllImport("user32.dll")] public static extern bool SetCursorPos(int x,int y);
  [DllImport("user32.dll")] public static extern void mouse_event(uint f,uint dx,uint dy,uint d,UIntPtr e);
  [DllImport("user32.dll")] public static extern void keybd_event(byte vk,byte sc,uint f,UIntPtr e);
  public struct RECT { public int Left,Top,Right,Bottom; }
  public struct POINT { public int X,Y; }
  public static void KeyDown(byte vk){ keybd_event(vk,0,0,UIntPtr.Zero); }
  public static void KeyUp(byte vk){ keybd_event(vk,0,2,UIntPtr.Zero); }
  public static void Move(IntPtr h,int cx,int cy){ POINT p=new POINT(); p.X=cx;p.Y=cy; ClientToScreen(h,ref p); SetCursorPos(p.X,p.Y); }
  public static void Click(IntPtr h,int cx,int cy){ Move(h,cx,cy); System.Threading.Thread.Sleep(200);
     mouse_event(0x0002,0,0,0,UIntPtr.Zero); System.Threading.Thread.Sleep(100); mouse_event(0x0004,0,0,0,UIntPtr.Zero); }
}
"@
$dir="C:\OpenCode-Drive\SharedF\nectariswin\game"
$p=Start-Process -FilePath "$dir\Nec.exe" -WorkingDirectory $dir -PassThru
Start-Sleep -Seconds 5
$p.Refresh(); if($p.HasExited){"EXITED";exit}
$h=$p.MainWindowHandle
$ws=New-Object -ComObject WScript.Shell
"activate=" + $ws.AppActivate($p.Id)
Start-Sleep -Milliseconds 600
function Snap($n){
  $r=New-Object W6+RECT; [void][W6]::GetWindowRect($h,[ref]$r)
  $w=$r.Right-$r.Left; $ht=$r.Bottom-$r.Top
  $bmp=New-Object System.Drawing.Bitmap($w,$ht)
  $g=[System.Drawing.Graphics]::FromImage($bmp); $dc=$g.GetHdc()
  $ok=[W6]::PrintWindow($h,$dc,2); $g.ReleaseHdc($dc); $g.Dispose()
  $bmp.Save("C:\OpenCode-Drive\SharedF\nectariswin\shot_$n.png",[System.Drawing.Imaging.ImageFormat]::Png); $bmp.Dispose()
  "snap $n print=$ok"
}
Snap "b0"
[W6]::Move($h,350,404); Start-Sleep -Milliseconds 700; Snap "b1_hover_load"
[W6]::Move($h,192,354); Start-Sleep -Milliseconds 700; Snap "b2_hover_start"
[W6]::Click($h,192,354); Start-Sleep -Milliseconds 2500; Snap "b3_click_start"
Start-Sleep -Milliseconds 2000; Snap "b4_later"
$p.Refresh(); if(-not $p.HasExited){Stop-Process -Id $p.Id -Force;"killed"}else{"exited $($p.ExitCode)"}
