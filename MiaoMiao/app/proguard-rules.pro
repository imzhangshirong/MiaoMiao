# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile

-keep class okhttp3.** { *; }
-keep interface okhttp3.** { *; }
-dontwarn okhttp3.**

-keep class okio.** { *; }
-keep interface okio.** { *; }
-dontwarn okio.**

-keep class com.squareup.picasso.** { *; }
-keep interface com.squareup.picasso.** { *; }
-dontwarn com.squareup.picasso.**

-keep class com.github.dmytrodanylyk.shadow-layout.** { *; }
-keep interface com.github.dmytrodanylyk.shadow-layout.** { *; }
-dontwarn com.github.dmytrodanylyk.shadow-layout.**

-keep class com.zhangshirong.miaomiao.DataBinding.** {*; }
-keep interface com.zhangshirong.miaomiao.DataBinding.** { *; }
-dontwarn com.zhangshirong.miaomiao.DataBinding.**

-keepclassmembers enum * {
    public static **[] values();
    public static ** valueOf(java.lang.String);
}