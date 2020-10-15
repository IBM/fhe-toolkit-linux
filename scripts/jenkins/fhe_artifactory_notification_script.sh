

SLACK_HOOK=$1
BUILD_KIND=$2
BUILD_TYPE=$3
ARTE_URL=$4

generate_post_message()
{
  cat <<EOF
{
  "text":"A New Build of $BUILD_KIND for $BUILD_TYPE based on Github commit:$COMMIT_NUMBER, passed all the tests and is now available for docker pull at <$ARTE_URL>"
}
EOF
}



COMMIT_NUMBER="$(git log -1 --pretty=format:"%h")"

#NOW=$(date +'%m-%d-%Y')
#NIGHTLY_SUFFIX="nightly-${NOW}"
#ARTE_URL="sys-ibm-fhe-team-linux-docker-local.artifactory.swg-devops.com/ubuntu/fhe-toolkit-ubuntu-amd64:$VERSION-$NIGHTLY_SUFFIX"
#MESSAGE='A New Build of "'"$BUILD_KIND"'" for "'"$BUILD_TYPE"'" based on Github commit:"'"$COMMIT_NUMBER"'", passed all the tests and is now available for docker pull at <"'"$ARTE_URL"'">'
#TEXT='{"text":"$MESSAGE"}'

echo $(generate_post_message)
curl -X POST -H 'Content-type: application/json' --data "$(generate_post_message)" $SLACK_HOOK

