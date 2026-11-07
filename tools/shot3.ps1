Add-Type -AssemblyName System.Drawing
Add-Type @"
using System;
using System.Runtime.InteropServices;
public class W3 {
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
  [DllImport("user32.dll")] public static extern bool PrintWindow(IntPtr h, IntPtr dc, uint flags);
  [DllImport("user32.dll")] public static extern bool PostMessage(IntPtr h, uint msg, IntPtr w, IntPtr l);
  public struct RECT { public int Left, Top, Right, Bottom; }
  public static void Key(IntPtr h, byte vk){
     PostMessage(h,0x0100,(IntPtr)vk,(IntPtr)1);
     System.Threading.Thread.Sleep(80);
     PostMessage(h,0x0101,(IntPtr)vk,(IntPtr)0xC0000001);
  }
  public static void Click(IntPtr h,int x,int y){
     IntPtr lp=(IntPtr)((y<<16)|(x&0xFFFF));
     PostMessage(h,0x0200,IntPtr.Zero,lp);
     PostMessage(h,0x0201,(IntPtr)1,lp);
     PostMessage(h,0x0202,IntPtr.Zero,lp);
  }
}
"@
function Grab($h,$path){
  $r=New-Object W3+RECT; [void][W3]::GetWindowRect($h,[ref]$r)
  $w=$r.Right-$r.Left; $ht=$r.Bottom-$r.Top
  $bmp=New-Object System.Drawing.Bitmap($w,$ht)
  $g=[System.Drawing.Graphics]::FromImage($bmp); $dc=$g.GetHdc()
  $ok=[W3]::PrintWindow($h,$dc,2); $g.ReleaseHdc($dc); $g.Dispose()
  $bmp.Save($path,[System.Drawing.Imaging.ImageFormat]::Png); $bmp.Dispose()
  return "print=$ok ${w}x${ht}"
}
$dir="C:\OpenCode-Drive\SharedF\nectariswin\game"
$p=Start-Process -FilePath "$dir\Nec.exe" -WorkingDirectory $dir -PassThru
Start-Sleep -Seconds 3
$p.Refresh(); if($p.HasExited){"EXITED";exit}
$h=$p.MainWindowHandle
"grab0 " + (Grab $h "C:\OpenCode-Drive\SharedF\nectariswin\shot_t0.png")
[W3]::Key($h,0x0D)
Start-Sleep -Milliseconds 1200
"grab1 " + (Grab $h "C:\OpenCode-Drive\SharedF\nectariswin\shot_t1.png")
[W3]::Key($h,0x0D)
Start-Sleep -Milliseconds 1200
"grab2 " + (Grab $h "C:\OpenCode-Drive\SharedF\nectariswin\shot_t2.png")
Start-Sleep -Milliseconds 300
$p.Refresh(); if(-not $p.HasExited){Stop-Process -Id $p.Id -Force;"killed"}else{"exited $($p.ExitCode)"}
