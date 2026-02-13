# GitHub Actions Workflows

This directory contains automated workflows for building and deploying the WarriorSim TBC project.

## Workflows

### 1. Build and Deploy (`build-and-deploy.yml`)

**Triggers:**
- Push to `main` or `master` branch
- Pull requests to `main` or `master` branch
- Manual trigger via GitHub Actions UI

**What it does:**
1. Sets up Emscripten for WebAssembly compilation
2. Installs Node.js dependencies
3. Builds the C++ backend to WebAssembly
4. Builds the Vue.js frontend
5. Deploys to GitHub Pages (on main/master branch only)

**Setup Required:**
1. Go to your repository Settings → Pages
2. Under "Build and deployment", select "GitHub Actions" as the source
3. Push to main/master branch to trigger deployment
4. Your site will be available at `https://[username].github.io/[repo-name]/`

### 2. Build Release (`build-release.yml`)

**Triggers:**
- Push of version tags (e.g., `v1.0.0`)
- Manual trigger via GitHub Actions UI

**What it does:**
1. Builds the complete project
2. Creates downloadable archives (.tar.gz and .zip)
3. Uploads build artifacts (available for 90 days)
4. Creates a GitHub Release with the archives (when triggered by tag)

**Usage:**
```bash
# Create and push a version tag
git tag v1.0.0
git push origin v1.0.0

# Or manually trigger from GitHub Actions UI
```

## Build Process

Both workflows follow the same build process:

### Backend (C++ → WebAssembly)
```bash
make
```
This compiles `src/main.cpp` and all headers into:
- `dist/js/warriorsim.js` - JavaScript wrapper
- `dist/js/warriorsim.wasm` - WebAssembly binary

### Frontend (Vue.js)
```bash
npx mix --production
```
This compiles:
- `assets/js/app.js` → `dist/js/app.js`
- `assets/sass/app.scss` → `dist/css/app.css`

## Local Development

To build locally:

```bash
# Install dependencies
npm install

# Build backend (requires Emscripten)
make

# Build frontend
npx mix

# Serve locally
python3 -m http.server
```

## Troubleshooting

### Build fails with "warriorsim.js not found"
- Check that Emscripten is properly installed
- Verify the Makefile is using the correct output path
- Check for C++ compilation errors in the logs

### Build fails with "npm ci" errors
- Delete `node_modules` and `package-lock.json`
- Run `npm install` locally to regenerate lock file
- Commit the updated `package-lock.json`

### GitHub Pages shows 404
- Ensure GitHub Pages is enabled in repository settings
- Check that the workflow completed successfully
- Verify the `index.html` is in the root directory
- Wait a few minutes for GitHub Pages to update

### WebAssembly fails to load
- Check browser console for CORS errors
- Ensure files are served via HTTP/HTTPS (not file://)
- Verify `warriorsim.wasm` is in `dist/js/` directory

## Caching

The workflows use caching to speed up builds:
- **Emscripten**: Cached in `emsdk-cache` directory
- **npm packages**: Cached based on `package-lock.json`

To clear caches:
1. Go to Actions → Caches in your repository
2. Delete the relevant caches
3. Re-run the workflow

## Permissions

The workflows require these permissions:
- `contents: read` - Read repository contents
- `pages: write` - Deploy to GitHub Pages
- `id-token: write` - Authenticate with GitHub Pages

These are configured in the workflow files.

## Environment Variables

No environment variables or secrets are required for basic operation.

If you want to customize the build:
- Add secrets in Settings → Secrets and variables → Actions
- Reference them in workflows with `${{ secrets.SECRET_NAME }}`

## Manual Triggers

Both workflows can be manually triggered:
1. Go to Actions tab in your repository
2. Select the workflow
3. Click "Run workflow"
4. Choose the branch
5. Click "Run workflow" button

## Artifacts

Build artifacts are available for download:
- **Build and Deploy**: Uploaded to GitHub Pages
- **Build Release**: Available in Actions → Workflow run → Artifacts
- **Tagged Release**: Available in Releases section

Artifacts are retained for 90 days by default.
