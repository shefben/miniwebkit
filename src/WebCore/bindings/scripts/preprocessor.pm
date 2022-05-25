#
# Copyright (C) 2005 Nikolas Zimmermann <wildfox@kde.org>
# Copyright (C) 2011 Google Inc.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with this library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
#

use strict;
use warnings;

use IPC::Open2;
use File::Basename;

BEGIN {
   use Exporter   ();
   our ($VERSION, @ISA, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);
   $VERSION     = 1.00;
   @ISA         = qw(Exporter);
   @EXPORT      = qw(&applyPreprocessor);
   %EXPORT_TAGS = ( );
   @EXPORT_OK   = ();
}

# Returns an array of lines.
sub applyPreprocessor
{
    my $fileName = shift;
    my $defines = shift;
    my $preprocessor = shift;
    my $useCl = 0;
    if (!$preprocessor) {
        require Config;
        my $gccLocation = "";
        if ($ENV{CC}) {
            $gccLocation = $ENV{CC};
        } elsif (($Config::Config{'osname'}) =~ /solaris/i) {
            $gccLocation = "/usr/sfw/bin/gcc";
        } elsif (($Config::Config{'osname'}) =~ /win/i) {
            if ($ENV{CL}) {
                $preprocessor = "\"".$ENV{CL}."\" /E /EP";
                $useCl = 1;
            } else {
                $gccLocation = "gcc";
            }
        } else {
            $gccLocation = "/usr/bin/gcc"
        }
        if (!$preprocessor) {
            $preprocessor = $gccLocation . " -E -P -x c++";
        }
    }

    if (!$defines) {
        $defines = "";
    }
    # my $LOG;
    # my $log = "preprocessor.log";
    # open LOG, ">>$log";
    my @definesArr1 = split(/\s+/, $defines);
    my @defineArgs;
    if ($useCl==1) {
        @defineArgs = map {"/D ".$_} @definesArr1;
    } else {
        @defineArgs = map {"-D".$_} @definesArr1;
    }
    my @definesStr = join(" ", @defineArgs);
    my $cmd = "$preprocessor @definesStr $fileName";
    my $pid = open2(\*PP_OUT, \*PP_IN, $cmd);
    close PP_IN;
    my @documentContent = <PP_OUT>;
    close PP_OUT;
    waitpid($pid, 0);
    # my $F;
    # my $output = "build/".basename($fileName).".i";
    # open F, ">$output";
    # print F @documentContent;
    return @documentContent;
}

1;
