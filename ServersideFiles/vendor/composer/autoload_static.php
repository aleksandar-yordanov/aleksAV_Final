<?php

// autoload_static.php @generated by Composer

namespace Composer\Autoload;

class ComposerStaticInitdd43ae85851521785956fdd777e49d1d
{
    public static $prefixLengthsPsr4 = array (
        'P' => 
        array (
            'PHPMailer\\PHPMailer\\' => 20,
        ),
    );

    public static $prefixDirsPsr4 = array (
        'PHPMailer\\PHPMailer\\' => 
        array (
            0 => __DIR__ . '/..' . '/phpmailer/phpmailer/src',
        ),
    );

    public static $classMap = array (
        'Composer\\InstalledVersions' => __DIR__ . '/..' . '/composer/InstalledVersions.php',
    );

    public static function getInitializer(ClassLoader $loader)
    {
        return \Closure::bind(function () use ($loader) {
            $loader->prefixLengthsPsr4 = ComposerStaticInitdd43ae85851521785956fdd777e49d1d::$prefixLengthsPsr4;
            $loader->prefixDirsPsr4 = ComposerStaticInitdd43ae85851521785956fdd777e49d1d::$prefixDirsPsr4;
            $loader->classMap = ComposerStaticInitdd43ae85851521785956fdd777e49d1d::$classMap;

        }, null, ClassLoader::class);
    }
}
